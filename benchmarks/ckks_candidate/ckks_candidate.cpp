#include "fheco/fheco.hpp"
#include <cctype>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using namespace fheco;

// Compiles an arbitrary CKKS program, given as an S-expression, through the
// same CHEHAB pipeline as the hand-written CKKS benchmarks (quad_ckks,
// chebyshev_ckks, relu_ckks, ...). Used by the LLM agent (llm_agent/) to
// evaluate candidate programs for real instead of estimating them.
//
// Inputs come from fhe_io_example.txt in the working directory (written by the
// benchmark's own generator, with independently computed expected outputs):
// every input label is a ciphertext or a plaintext input of the function, and
// the single output label names the result. The generated he/main checks the
// decrypted result against those expected outputs.
//
// Grammar (every operation applies to all slots at once):
//   expr := label
//         | (+ expr expr ...) | (* expr expr ...)      n-ary, folded left
//         | (- expr expr) | (- expr)                    subtraction / negation
//         | (let name expr expr)                        bind name in the body
// At least one operand of every operation must be a ciphertext (plaintext
// inputs only exist at run time).

namespace
{
struct Node
{
  string atom; // label when args is empty
  vector<unique_ptr<Node>> args;
  bool is_atom() const { return args.empty() && !atom.empty() && atom != "("; }
};

vector<string> tokenize(const string &s)
{
  vector<string> tokens;
  string cur;
  for (char ch : s)
  {
    if (ch == '(' || ch == ')')
    {
      if (!cur.empty())
        tokens.push_back(cur), cur.clear();
      tokens.emplace_back(1, ch);
    }
    else if (isspace(static_cast<unsigned char>(ch)))
    {
      if (!cur.empty())
        tokens.push_back(cur), cur.clear();
    }
    else
      cur += ch;
  }
  if (!cur.empty())
    tokens.push_back(cur);
  return tokens;
}

unique_ptr<Node> parse(const vector<string> &tokens, size_t &pos)
{
  if (pos >= tokens.size())
    throw invalid_argument("unexpected end of expression");
  const string &tok = tokens[pos++];
  auto node = make_unique<Node>();
  if (tok == ")")
    throw invalid_argument("unexpected ')'");
  if (tok != "(")
  {
    node->atom = tok;
    return node;
  }
  if (pos >= tokens.size() || tokens[pos] == "(" || tokens[pos] == ")")
    throw invalid_argument("'(' must be followed by an operator");
  node->atom = tokens[pos++];
  // Mark as an operation even when it has no arguments.
  while (pos < tokens.size() && tokens[pos] != ")")
    node->args.push_back(parse(tokens, pos));
  if (pos >= tokens.size())
    throw invalid_argument("missing ')' for operator '" + node->atom + "'");
  ++pos;
  if (node->args.empty())
    throw invalid_argument("operator '" + node->atom + "' has no arguments");
  return node;
}

struct Value
{
  bool is_cipher = false;
  shared_ptr<Ciphertext> c;
  shared_ptr<Plaintext> p;
};

using Env = unordered_map<string, Value>;

Value binary(const string &op, const Value &a, const Value &b)
{
  if (!a.is_cipher && !b.is_cipher)
    throw invalid_argument(
      "operation '" + op + "' between two plaintext operands: at least one operand must depend on a ciphertext");
  Value r;
  r.is_cipher = true;
  if (op == "+")
    r.c = make_shared<Ciphertext>(
      a.is_cipher ? (b.is_cipher ? *a.c + *b.c : *a.c + *b.p) : *a.p + *b.c);
  else if (op == "-")
    r.c = make_shared<Ciphertext>(
      a.is_cipher ? (b.is_cipher ? *a.c - *b.c : *a.c - *b.p) : *a.p - *b.c);
  else if (op == "*")
    r.c = make_shared<Ciphertext>(
      a.is_cipher ? (b.is_cipher ? *a.c * *b.c : *a.c * *b.p) : *a.p * *b.c);
  else
    throw invalid_argument("unknown operator '" + op + "'");
  return r;
}

Value eval(const Node &n, Env &env)
{
  if (n.args.empty())
  {
    auto it = env.find(n.atom);
    if (it == env.end())
      throw invalid_argument("unknown variable '" + n.atom + "'");
    return it->second;
  }
  const string &op = n.atom;
  if (op == "let")
  {
    if (n.args.size() != 3 || !n.args[0]->args.empty())
      throw invalid_argument("let expects (let name value body)");
    const string &name = n.args[0]->atom;
    Value v = eval(*n.args[1], env);
    auto saved = env.find(name);
    bool had = saved != env.end();
    Value old = had ? saved->second : Value{};
    env[name] = v;
    Value body = eval(*n.args[2], env);
    if (had)
      env[name] = old;
    else
      env.erase(name);
    return body;
  }
  if (op == "-" && n.args.size() == 1)
  {
    Value a = eval(*n.args[0], env);
    if (!a.is_cipher)
      throw invalid_argument("negation of a plaintext operand: the operand must depend on a ciphertext");
    Value r;
    r.is_cipher = true;
    r.c = make_shared<Ciphertext>(-*a.c);
    return r;
  }
  if (op == "-" && n.args.size() != 2)
    throw invalid_argument("'-' expects one (negation) or two (subtraction) arguments");
  if (op != "+" && op != "-" && op != "*")
    throw invalid_argument("unknown operator '" + op + "' (allowed: + - * let)");
  if (n.args.size() < 2)
    throw invalid_argument("'" + op + "' expects at least two arguments");
  Value acc = eval(*n.args[0], env);
  for (size_t i = 1; i < n.args.size(); ++i)
    acc = binary(op, acc, eval(*n.args[i], env));
  return acc;
}

struct IoLabels
{
  vector<pair<string, bool>> inputs; // label, is_cipher
  string output;
};

IoLabels read_io_labels(const string &path)
{
  ifstream is(path);
  if (!is)
    throw invalid_argument("failed to open " + path);
  size_t slots, nb_inputs, nb_outputs;
  string line;
  getline(is, line);
  istringstream header(line);
  if (!(header >> slots >> nb_inputs >> nb_outputs))
    throw invalid_argument("malformed io header");
  if (nb_outputs != 1)
    throw invalid_argument("ckks_candidate supports exactly one output");
  IoLabels io;
  for (size_t i = 0; i < nb_inputs; ++i)
  {
    getline(is, line);
    istringstream ls(line);
    string label;
    int is_cipher;
    ls >> label >> is_cipher;
    io.inputs.emplace_back(label, is_cipher == 1);
  }
  getline(is, line);
  istringstream ls(line);
  ls >> io.output;
  return io;
}
} // namespace

int main(int argc, char **argv)
{
  // ./ckks_candidate <expr_file> [slot_count] [call_quantifier] [cse] [const_folding]
  if (argc < 2)
  {
    cerr << "usage: " << argv[0] << " <expr_file> [slot_count] [call_quantifier] [cse] [const_folding]\n";
    return 2;
  }
  string expr_file = argv[1];
  size_t slot_count = argc > 2 ? stoul(argv[2]) : 8;
  bool call_quantifier = argc > 3 ? stoi(argv[3]) : true;
  bool cse = argc > 4 ? stoi(argv[4]) : true;
  bool const_folding = argc > 5 ? stoi(argv[5]) : true;

  if (cse)
  {
    Compiler::enable_cse();
    Compiler::enable_order_operands();
  }
  else
  {
    Compiler::disable_cse();
    Compiler::disable_order_operands();
  }
  if (const_folding)
    Compiler::enable_const_folding();
  else
    Compiler::disable_const_folding();

  ifstream es(expr_file);
  if (!es)
  {
    cerr << "failed to open " << expr_file << "\n";
    return 2;
  }
  stringstream buf;
  buf << es.rdbuf();

  try
  {
    auto tokens = tokenize(buf.str());
    size_t pos = 0;
    auto root = parse(tokens, pos);
    if (pos != tokens.size())
      throw invalid_argument("unexpected trailing tokens after the expression");

    IoLabels io = read_io_labels("fhe_io_example.txt");

    // Same CKKS parameters as every other CHEHAB CKKS benchmark.
    CkksParams params{32768, {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}, 40};

    chrono::high_resolution_clock::time_point t = chrono::high_resolution_clock::now();
    string func_name = "fhe";
    const auto &func = Compiler::create_ckks_func(func_name, slot_count, params);
    util::copyFile("fhe_io_example.txt", "fhe_io_example_adapted.txt");

    Env env;
    for (const auto &[label, is_cipher] : io.inputs)
    {
      Value v;
      v.is_cipher = is_cipher;
      if (is_cipher)
        v.c = make_shared<Ciphertext>(label);
      else
        v.p = make_shared<Plaintext>(label);
      env.emplace(label, v);
    }
    Value result = eval(*root, env);
    if (!result.is_cipher)
      throw invalid_argument("the expression must evaluate to a ciphertext");
    result.c->set_output(io.output);

    string gen_name = "_gen_he_" + func_name;
    string gen_path = "he/" + gen_name;
    ofstream header_os(gen_path + ".hpp");
    if (!header_os)
      throw logic_error("failed to create header file");
    ofstream source_os(gen_path + ".cpp");
    if (!source_os)
      throw logic_error("failed to create source file");
    Compiler::compile(func, Compiler::Ruleset::simplification_ruleset, trs::RewriteHeuristic::bottom_up);
    Compiler::gen_he_code(func, header_os, gen_name + ".hpp", source_os);
    chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - t;
    cout << "Compile time : \n" << elapsed.count() << " ms\n";
    if (call_quantifier)
    {
      util::Quantifier quantifier{func};
      quantifier.run_all_analysis();
      quantifier.print_info(cout);
    }
  }
  catch (const exception &e)
  {
    cerr << "candidate error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
