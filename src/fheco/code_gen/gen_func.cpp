#include "fheco/code_gen/constants.hpp"
#include "fheco/code_gen/gen_func.hpp"
#include "fheco/ir/common.hpp"
#include "fheco/ir/func.hpp"
#include "fheco/passes/prepare_code_gen.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
using namespace std;
  
namespace fheco::code_gen
{

void gen_func(
  const shared_ptr<ir::Func> &func, const unordered_set<int> &rotataion_steps, ostream &header_os,
  string_view header_name, ostream &source_os,param_select::EncParams::SecurityLevel security_level,bool automatic_enc_params_enabled)
{
  const bool ckks = func->scheme() == Scheme::ckks;
  if (ckks)
  {
    // Real-valued IR constants are not supported; real values enter CKKS
    // functions only as named plaintext inputs.
    if (!func->data_flow().constants_info().empty())
      throw logic_error("CKKS functions do not support constants yet");

    for (const auto &output_info : func->data_flow().outputs_info())
    {
      if (output_info.first->type() != ir::Term::Type::cipher)
        throw logic_error("CKKS functions do not support plaintext outputs yet");
    }
  }

  passes::prepare_code_gen(func);
  header_os << header_includes;
  header_os << '\n';
  gen_func_decl(func->name(), func->scheme(), header_os);
  header_os << '\n';
  gen_rotation_steps_getter_decl(func->name(), header_os);
  /*************************************************************/
  /*************************************************************/
  // CKKS scale-alignment helpers are needed by match_scale terms and by
  // ciphertext-ciphertext additions/subtractions.
  bool need_ckks_scale_helpers = false;
  bool need_ckks_encode_helper = false;
  if (ckks)
  {
    for (auto term : func->get_top_sorted_terms())
    {
      const auto op_type = term->op_code().type();
      if (term->type() == ir::Term::Type::cipher)
      {
        for (auto operand : term->operands())
          need_ckks_encode_helper = need_ckks_encode_helper || operand->type() == ir::Term::Type::plain;
      }
      if (op_type == ir::OpCode::Type::match_scale)
        need_ckks_scale_helpers = true;
      else if (
        (op_type == ir::OpCode::Type::add || op_type == ir::OpCode::Type::sub) &&
        term->operands()[0]->type() == ir::Term::Type::cipher && term->operands()[1]->type() == ir::Term::Type::cipher)
        need_ckks_scale_helpers = true;
    }
  }

  source_os << source_includes;
  if (ckks)
  {
    source_os << "#include <cmath>\n";
    if (need_ckks_scale_helpers)
      source_os << "#include <algorithm>\n#include <stdexcept>\n";
  }
  source_os << "#include \"" << header_name << "\"\n";
  source_os << '\n';
  source_os << source_usings;
  source_os << '\n';
  if (need_ckks_scale_helpers)
    source_os << ckks_scale_helpers << '\n';
  if (need_ckks_encode_helper)
    source_os << ckks_encode_helper << '\n';
  gen_func_def_signature(func->name(), func->scheme(), source_os);
  source_os << "\n{\n";

  if (ckks)
  {
    // Nominal CKKS scale, used to encode plaintext multiplicands.
    bool has_ctxt_ptxt_mul = false;
    for (auto term : func->get_top_sorted_terms())
    {
      if (term->op_code().type() == ir::OpCode::Type::mul && term->type() == ir::Term::Type::cipher)
      {
        for (auto operand : term->operands())
          has_ctxt_ptxt_mul = has_ctxt_ptxt_mul || operand->type() == ir::Term::Type::plain;
      }
    }
    if (has_ctxt_ptxt_mul)
      source_os << "const double " << ckks_scale_id << " = std::ldexp(1.0, " << func->ckks_params().log2_scale
                << ");\n";
  }

  TermsCtxtObjectsInfo terms_ctxt_objects_info;
  gen_input_terms(func->data_flow().inputs_info(), func->scheme(), source_os, terms_ctxt_objects_info);
  gen_const_terms(func->data_flow().constants_info(), func->clear_data_evaluator().signedness(), source_os);
  gen_op_terms(func, source_os, terms_ctxt_objects_info);
  gen_output_terms(func->data_flow().outputs_info(), source_os, terms_ctxt_objects_info);

  source_os << "}\n";
  source_os << '\n';
  gen_rotation_steps_getter_def(func->name(), rotataion_steps, source_os);
  source_os << '\n';
  /****************************************************************/
  if (ckks)
  {
    // CKKS parameters are given explicitly; the parameter selector is BFV-only.
    gen_main_code_ckks(func->ckks_params(), security_level);
    return;
  }
  //std::cout<<"\n ==>Welcome in encryption params selection : \n";
  param_select::ParameterSelector selector(func, security_level);
  bool use_mod_switch = false ;
  if(automatic_enc_params_enabled){
    param_select::EncParams params = selector.select_params(use_mod_switch); 
    params.print_params(std::cout); 
    //std::cout<<"Encryption params have been printed succefully\n";
    gen_main_code(params,security_level,automatic_enc_params_enabled);
  }else{ 
    int poly_modulus_degree = 16384 ; 
    param_select::EncParams params = param_select::EncParams(poly_modulus_degree,func->plain_modulus()); 
    gen_main_code(params,security_level,automatic_enc_params_enabled);
  }
  //std::cout<<"Gen main code has been done \n";
}

/**************************************************************************************/
/**************************************************************************************/

void gen_func_decl(const string &func_name, Scheme scheme, ostream &os)
{
  const bool ckks = scheme == Scheme::ckks;
  const string_view encoder_type_name = ckks ? ckks_encoder_type : encoder_type;
  os << "void " << func_name << "("; 
  os << "const " << header_encrypted_io_type << " &" << encrypted_inputs_container_id << ",\n";
  if (ckks)
    os << "const " << header_ckks_plain_io_type << " &" << ckks_plain_inputs_container_id << ",\n";
  else
    os << "const " << header_encoded_io_type << " &" << encoded_inputs_container_id << ",\n";
  os << header_encrypted_io_type << " &" << encrypted_outputs_container_id << ",\n";
  os << header_encoded_io_type << " &" << encoded_outputs_container_id << ",\n";
  os << "const " << seal_namespace << "::" << encoder_type_name << " &" << encoder_id << ",\n";
  os << "const " << seal_namespace << "::" << encryptor_type << " &" << encryptor_id << ",\n";
  os << "const " << seal_namespace << "::" << evaluator_type << " &" << evaluator_id << ",\n";
  os << "const " << seal_namespace << "::" << relin_keys_type << " &" << relin_keys_id << ",\n";
  os << "const " << seal_namespace << "::" << galois_keys_type << " &" << galois_keys_id;
  if (ckks)
    os << ",\nconst " << seal_namespace << "::" << context_type << " &" << context_id;
  os << ");\n";
}

void gen_rotation_steps_getter_decl(const string &func_name, ostream &os)
{
  os << "std::vector<int> " << rotation_steps_getter_id + "_" + func_name << "();\n";
}

void gen_func_def_signature(const string &func_name, Scheme scheme, ostream &os)
{ 
  const bool ckks = scheme == Scheme::ckks;
  const string_view encoder_type_name = ckks ? ckks_encoder_type : encoder_type;
  os << "void " << func_name << "(";
  os << "const " << source_encrypted_io_type << " &" << encrypted_inputs_container_id << ",\n";
  if (ckks)
    os << "const " << source_ckks_plain_io_type << " &" << ckks_plain_inputs_container_id << ",\n";
  else
    os << "const " << source_encoded_io_type << " &" << encoded_inputs_container_id << ",\n";
  os << source_encrypted_io_type << " &" << encrypted_outputs_container_id << ",\n";
  os << source_encoded_io_type << " &" << encoded_outputs_container_id << ",\n";
  os << "const " << encoder_type_name << " &" << encoder_id << ",\n";
  os << "const " << encryptor_type << " &" << encryptor_id << ",\n";
  os << "const " << evaluator_type << " &" << evaluator_id << ",\n";
  os << "const " << relin_keys_type << " &" << relin_keys_id << ",\n";
  os << "const " << galois_keys_type << " &" << galois_keys_id;
  if (ckks)
    os << ",\nconst " << context_type << " &" << context_id;
  os << ")";
}

void gen_input_terms(
  const ir::InputTermsInfo &input_terms_info, Scheme scheme, ostream &os, TermsCtxtObjectsInfo &terms_ctxt_objects_info)
{
  for (const auto &input_info : input_terms_info)
  {
    auto term = input_info.first;
    auto object_id = term->id();
    if (term->type() == ir::Term::Type::cipher)
    {
      terms_ctxt_objects_info.emplace(term->id(), CtxtObjectInfo{object_id, term->parents().size()});
      os << cipher_type << " ";
      gen_cipher_var_id(object_id, os);
      os << " = " << encrypted_inputs_container_id << ".at(\"" << input_info.second.label_ << "\")";
    }
    else if (scheme == Scheme::ckks)
    {
      // real values; encoded where used (see gen_op_terms)
      os << "const vector<double> &";
      gen_plain_var_id(object_id, os);
      os << " = " << ckks_plain_inputs_container_id << ".at(\"" << input_info.second.label_ << "\")";
    }
    else
    {
      os << plain_type << " ";
      gen_plain_var_id(object_id, os);
      os << " = " << encoded_inputs_container_id << ".at(\"" << input_info.second.label_ << "\")";
    }
    os << ";\n";
  }
}

void gen_cipher_var_id(size_t term_id, ostream &os)
{
  os << "c" << term_id;
}

void gen_plain_var_id(size_t term_id, ostream &os)
{
  os << "p" << term_id;
}

void gen_const_terms(const ir::ConstTermsValues &const_terms_info, bool signedness, ostream &os)
{
  if (const_terms_info.size())
    os << "size_t " << slot_count_id << " = " << encoder_id << ".slot_count();\n";
  for (const auto &const_info : const_terms_info)
  {
    auto term = const_info.first;
    auto object_id = term->id();
    os << plain_type << " ";
    gen_plain_var_id(object_id, os);
    os << ";\n";
    os << encoder_id << ".encode(vector<";
    if (signedness)
      os << signed_slot_type;
    else
      os << unsigned_slot_type;
    os << ">";
    if (const_info.second.is_scalar_)
      os << "(" << slot_count_id << ", " << const_info.second.val_[0] << "), ";
    else
    {
      os << "{";
      gen_sequence(const_info.second.val_.cbegin(), const_info.second.val_.cend(), line_threshold, os);
      os << "},\n";
    }
    gen_plain_var_id(object_id, os);
    os << ");\n";
  }
}

void gen_op_terms(const shared_ptr<ir::Func> &func, ostream &os, TermsCtxtObjectsInfo &terms_ctxt_objects_info)
{
  for (auto term : func->get_top_sorted_terms())
  {
    if (!term->is_operation()) 
      continue;

    auto term_object_id = term->id();
    vector<size_t> operands_ctxt_objects_ids(term->operands().size());
    unordered_map<size_t, size_t> operands_multip;
    for (size_t i = 0; i < operands_ctxt_objects_ids.size(); ++i)
    {
      auto operand = term->operands()[i];
      if (operand->type() != ir::Term::Type::cipher)
        continue;

      auto multip = ++operands_multip[operand->id()];
      auto operand_object_info_it = terms_ctxt_objects_info.find(operand->id());
      // operand with multiplicity > 1 selected for overwrite
      if (operand_object_info_it == terms_ctxt_objects_info.end())
      {
        operands_ctxt_objects_ids[i] = term_object_id;
        continue;
      }
      auto &operand_object_info = operand_object_info_it->second;
      operands_ctxt_objects_ids[i] = operand_object_info.id_;
      if (func->data_flow().is_output(operand) || operand->type() == ir::Term::Type::plain || multip > 1)
        continue;

      --operand_object_info.dep_count_;
      if (term_object_id == term->id() && operand_object_info.dep_count_ == 0)
      {
        term_object_id = operands_ctxt_objects_ids[i];
        terms_ctxt_objects_info.erase(operand_object_info_it);
      }
    }

    if (term_object_id == term->id())
    {
      for (auto it = terms_ctxt_objects_info.begin(); it != terms_ctxt_objects_info.end(); ++it)
      {
        if (it->second.dep_count_ == 0)
        {
          term_object_id = it->second.id_;
          terms_ctxt_objects_info.erase(it);
          break;
        }
      }
    }
    auto dep_count = term->parents().size();
    if (func->data_flow().is_output(term))
      ++dep_count;

    terms_ctxt_objects_info.emplace(term->id(), CtxtObjectInfo{term_object_id, dep_count});

    if (term_object_id == term->id())
    {
      os << cipher_type << " ";
      gen_cipher_var_id(term_object_id, os);
      os << ";\n";
    }

    // CKKS: encode each plaintext operand at the level of the ciphertext
    // operand. A multiplicand gets the nominal scale (the product is rescaled
    // by insert_rescale); an addend gets the ciphertext's exact scale.
    vector<string> ckks_encoded_plain_ids(term->operands().size());
    if (func->scheme() == Scheme::ckks)
    {
      if (term->op_code().type() == ir::OpCode::Type::encrypt)
        throw logic_error("CKKS functions do not support encrypting plaintexts yet");

      for (size_t i = 0; i < term->operands().size(); ++i)
      {
        auto operand = term->operands()[i];
        if (operand->type() != ir::Term::Type::plain)
          continue;

        const auto op_type = term->op_code().type();
        if (
          term->operands().size() != 2 ||
          (op_type != ir::OpCode::Type::mul && op_type != ir::OpCode::Type::add && op_type != ir::OpCode::Type::sub))
          throw logic_error(
            "CKKS functions only support plaintext operands of ciphertext-plaintext mul/add/sub, not '" +
            term->op_code().str_repr() + "'");

        const size_t ctxt_object_id = operands_ctxt_objects_ids[1 - i];
        const string encoded_id = "p" + to_string(operand->id()) + "_" + to_string(term->id());
        os << plain_type << " " << encoded_id << ";\n";
        os << ckks_encode_helper_id << "(" << encoder_id << ", ";
        gen_plain_var_id(operand->id(), os);
        os << ", ";
        gen_cipher_var_id(ctxt_object_id, os);
        os << ".parms_id(), ";
        if (op_type == ir::OpCode::Type::mul)
          os << ckks_scale_id;
        else
        {
          gen_cipher_var_id(ctxt_object_id, os);
          os << ".scale()";
        }
        os << ", " << encoded_id << ");\n";
        ckks_encoded_plain_ids[i] = encoded_id;
      }
    }

    vector<ir::Term::Type> operands_types;
    operands_types.reserve(term->operands().size());
    transform(
      term->operands().cbegin(), term->operands().cend(), back_inserter(operands_types),
      [](const ir::Term *operand) { return operand->type(); });

    if (func->scheme() == Scheme::ckks && term->op_code().type() == ir::OpCode::Type::match_scale)
    {
      // a * 1 encoded so that, after rescaling, the scale equals ref's exactly.
      // ref's scale is read first: ref's object may be the destination.
      const string target_scale_id = "target_scale_" + to_string(term->id());
      const string one_id = "one_" + to_string(term->id());
      os << "const double " << target_scale_id << " = ";
      gen_cipher_var_id(operands_ctxt_objects_ids[1], os);
      os << ".scale();\n";
      os << plain_type << " " << one_id << ";\n";
      os << encoder_id << ".encode(1.0, ";
      gen_cipher_var_id(operands_ctxt_objects_ids[0], os);
      os << ".parms_id(), ckks_landing_scale(" << context_id << ", ";
      gen_cipher_var_id(operands_ctxt_objects_ids[0], os);
      os << ", " << target_scale_id << "), " << one_id << ");\n";
      os << evaluator_id << ".multiply_plain(";
      gen_cipher_var_id(operands_ctxt_objects_ids[0], os);
      os << ", " << one_id << ", ";
      gen_cipher_var_id(term_object_id, os);
      os << ");\n";
      os << evaluator_id << ".rescale_to_next(";
      gen_cipher_var_id(term_object_id, os);
      os << ", ";
      gen_cipher_var_id(term_object_id, os);
      os << ");\n";
      os << "ckks_snap_scale(";
      gen_cipher_var_id(term_object_id, os);
      os << ", " << target_scale_id << ");\n";
      continue;
    }

    if (
      func->scheme() == Scheme::ckks &&
      (term->op_code().type() == ir::OpCode::Type::add || term->op_code().type() == ir::OpCode::Type::sub) &&
      term->operands()[0]->type() == ir::Term::Type::cipher && term->operands()[1]->type() == ir::Term::Type::cipher &&
      operands_ctxt_objects_ids[0] != operands_ctxt_objects_ids[1])
    {
      // align_ckks_operands made the scales equal; remove floating-point
      // rounding differences, which SEAL's add/sub would reject.
      os << "ckks_snap_scale(";
      gen_cipher_var_id(operands_ctxt_objects_ids[1], os);
      os << ", ";
      gen_cipher_var_id(operands_ctxt_objects_ids[0], os);
      os << ".scale());\n";
    }

    if (term->op_code() == ir::OpCode::encrypt)
    {
      os << encryptor_id << ".encrypt(";
      gen_plain_var_id(term->operands()[0]->id(), os);
    }
    else
    {
      os << evaluator_id << ".";
      if (func->scheme() == Scheme::ckks && term->op_code().type() == ir::OpCode::Type::rotate)
        os << "rotate_vector";
      else
        os << operation_mapping.at(ir::OpType{term->op_code().type(), move(operands_types)});
      os << "(";
      // operation term needs operands so operands_ctxt_objects_ids cannot be empty
      for (size_t i = 0;; ++i)
      {
        auto operand = term->operands()[i];

        if (operand->type() == ir::Term::Type::cipher)
          gen_cipher_var_id(operands_ctxt_objects_ids[i], os);
        else if (!ckks_encoded_plain_ids[i].empty())
          os << ckks_encoded_plain_ids[i];
        else
          gen_plain_var_id(operand->id(), os);

        if (i == operands_ctxt_objects_ids.size() - 1)
          break;

        os << ", ";
      }
      if (term->op_code().generators().size())
      {
        os << ", ";
        for (auto it = term->op_code().generators().cbegin();;)
        {
          auto generator = *it;
          os << generator;
          ++it;
          if (it == term->op_code().generators().cend())
            break;

          os << ", ";
        }
      }
      if (term->op_code().type() == ir::OpCode::Type::relin)
        os << ", " << relin_keys_id;

      if (term->op_code().type() == ir::OpCode::Type::rotate)
        os << ", " << galois_keys_id;
    }
    os << ", ";
    gen_cipher_var_id(term_object_id, os);
    os << ");\n";
  }
}

void gen_output_terms(
  const ir::OutputTermsInfo &output_terms_info, ostream &os, const TermsCtxtObjectsInfo &terms_ctxt_objects_info)
{
  for (const auto &output_info : output_terms_info)
  {
    auto term = output_info.first;
    if (term->type() == ir::Term::Type::cipher)
    {
      auto ctxt_object_id = terms_ctxt_objects_info.at(term->id()).id_;
      // an output term has at least one label
      for (auto it = output_info.second.labels_.cbegin();;)
      {
        const auto &label = *it;
        os << encrypted_outputs_container_id << ".emplace(\"" << label << "\", ";
        ++it;
        if (it == output_info.second.labels_.cend())
        {
          os << "move(";
          gen_cipher_var_id(ctxt_object_id, os);
          os << "));\n";
          break;
        }
        gen_cipher_var_id(ctxt_object_id, os);
        os << ");\n";
      }
    }
    else
    {
      for (auto it = output_info.second.labels_.cbegin();;)
      {
        const auto &label = *it;
        os << encoded_outputs_container_id << ".emplace(\"" << label << "\", ";
        ++it;
        if (it == output_info.second.labels_.cend())
        {
          os << "move(";
          gen_plain_var_id(term->id(), os);
          os << "));\n";
          break;
        }
        gen_plain_var_id(term->id(), os);
        os << ");\n";
      }
    }
  }
}

void gen_rotation_steps_getter_def(const string &func_name, const unordered_set<int> &steps, ostream &os)
{
  os << "vector<int> " << rotation_steps_getter_id + "_" + func_name << "(){\n";
  os << "return vector<int>{";
  gen_sequence(steps.cbegin(), steps.cend(), line_threshold, os);
  os << "};\n";
  os << "}\n";
}

void gen_main_code(fheco::param_select::EncParams params,param_select::EncParams::SecurityLevel security_level,bool automatic_enc_params_enabled)
{
    ofstream out("./he/main.cpp");
    string_view str_1{ 
            R"(#include <chrono>
    #include <cstddef>
    #include <fstream>
    #include <iostream>
    #include <ostream> 
    #include "_gen_he_fhe.hpp"
    #include "utils.hpp"

    using namespace std;
    using namespace seal;

    void print_bool_arg(bool arg, const string &name, ostream &os)
    {
      os << (arg ? name : "no" + name); 
    }

    int main(int argc, char **argv)
    {
      bool opt = true;
      if (argc > 1)
        opt = stoi(argv[1]);

      print_bool_arg(opt, "opt", clog);
      clog << '\n';

      string app_name = "fhe";
      ifstream is("../" + app_name + "_io_example_adapted.txt");
      if (!is)
        throw invalid_argument("failed to open io example file");
      )"
      };
      string str_1_converted{str_1};
      out<<str_1_converted ;
      out<<"  EncryptionParameters params(scheme_type::bfv); \n";
      out<<"  size_t n ="<<params.poly_mod_degree()<<" ;\n";
      out<<"  params.set_poly_modulus_degree(n);\n";
      out<<"  params.set_plain_modulus(PlainModulus::Batching(n, "<<params.plain_mod_bit_size()<<")); \n";
      if(automatic_enc_params_enabled){
        out<<"  params.set_coeff_modulus(CoeffModulus::Create(n, {";
        gen_sequence(params.coeff_mod_bit_sizes().cbegin(),params.coeff_mod_bit_sizes().cend(), line_threshold, out);
        out<<" }));\n";
      }else{
        out<<"params.set_coeff_modulus(CoeffModulus::BFVDefault("<<params.poly_mod_degree()<<"));\n";
      }
      string security_level_label = "tc128";
      switch (security_level){
        case param_select::EncParams::SecurityLevel::tc128 :
            security_level_label = "tc128";
            break;
        case param_select::EncParams::SecurityLevel::tc192 :
            security_level_label = "tc192";
            break;
        case param_select::EncParams::SecurityLevel::tc256 :
            security_level_label = "tc256";
            break;
        default :
          throw invalid_argument("Unsupported Security level");
      }
      out<<"  SEALContext context(params, true, sec_level_type::"<<security_level_label<<");\n";
      string_view str_2{ 
            R"(
      ClearArgsInfo clear_inputs, clear_outputs;
      size_t func_slot_count;
      BatchEncoder batch_encoder(context);
      KeyGenerator keygen(context);
      const SecretKey &secret_key = keygen.secret_key();
      PublicKey public_key;
      keygen.create_public_key(public_key);
      RelinKeys relin_keys;
      keygen.create_relin_keys(relin_keys);
      GaloisKeys galois_keys; 
      keygen.create_galois_keys(galois_keys);
      //keygen.create_galois_keys(get_rotation_steps_fhe(), galois_keys);
      Encryptor encryptor(context, public_key);
      Evaluator evaluator(context);
      Decryptor decryptor(context, secret_key);
      EncryptedArgs encrypted_inputs;
      EncodedArgs encoded_inputs;
      
      chrono::high_resolution_clock::time_point t;
      chrono::duration<double, milli> elapsed;
      t = chrono::high_resolution_clock::now();

      parse_inputs_outputs_file(is, params.plain_modulus().value(), clear_inputs, clear_outputs, func_slot_count);
      prepare_he_inputs(batch_encoder, encryptor, clear_inputs, encrypted_inputs, encoded_inputs);
      EncryptedArgs encrypted_outputs;
      EncodedArgs encoded_outputs;

      fhe(
        encrypted_inputs, encoded_inputs, encrypted_outputs, encoded_outputs, batch_encoder, encryptor, evaluator,
        relin_keys, galois_keys);

      elapsed = chrono::high_resolution_clock::now() - t;

      ClearArgsInfo obtained_clear_outputs;
      get_clear_outputs(
        batch_encoder, decryptor, encrypted_outputs, encoded_outputs, func_slot_count, obtained_clear_outputs);
      print_encrypted_outputs_info(context, decryptor, encrypted_outputs, clog);
      cout <<"execution_time_(ms): "<<elapsed.count() <<"\n"<<std::flush;
    }
    )"
    }; 
      string str_2_converted{str_2};
      out<<str_2_converted ;
      out.close();
  }

void gen_main_code_ckks(const CkksParams &params, param_select::EncParams::SecurityLevel security_level)
{
  string security_level_label;
  switch (security_level)
  {
  case param_select::EncParams::SecurityLevel::tc128:
    security_level_label = "tc128";
    break;
  case param_select::EncParams::SecurityLevel::tc192:
    security_level_label = "tc192";
    break;
  case param_select::EncParams::SecurityLevel::tc256:
    security_level_label = "tc256";
    break;
  default:
    throw invalid_argument("Unsupported Security level");
  }

  ofstream out("./he/main.cpp");
  if (!out)
    throw logic_error("failed to create he/main.cpp");

  out << R"(#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "_gen_he_fhe.hpp"
#include "ckks_utils.hpp"

using namespace std;
using namespace seal;

int main(int argc, char **argv)
{
  // Absolute tolerance for |decrypted output - expected output|.
  double tolerance = 1e-6;
  // Timed fhe() runs after one warmup run, for the evaluation-only timing.
  size_t repeats = 10;
  for (int i = 1; i < argc; ++i)
  {
    string arg = argv[i];
    if (arg == "--tol" && i + 1 < argc)
      tolerance = stod(argv[++i]);
    else if (arg == "--repeats" && i + 1 < argc)
      repeats = stoul(argv[++i]);
    else
    {
      cerr << "usage: " << argv[0] << " [--tol T] [--repeats N]\n";
      return 2;
    }
  }

  string app_name = "fhe";
  ifstream is("../" + app_name + "_io_example_adapted.txt");
  if (!is)
    throw invalid_argument("failed to open io example file");

  EncryptionParameters params(scheme_type::ckks);
)";
  out << "  size_t n = " << params.poly_modulus_degree << ";\n";
  out << "  params.set_poly_modulus_degree(n);\n";
  out << "  params.set_coeff_modulus(CoeffModulus::Create(n, {";
  gen_sequence(params.coeff_mod_bit_sizes.cbegin(), params.coeff_mod_bit_sizes.cend(), line_threshold, out);
  out << "}));\n";
  out << "  SEALContext context(params, true, sec_level_type::" << security_level_label << ");\n";
  out << "  const double scale = pow(2.0, " << params.log2_scale << ");\n";
  out << R"(
  CKKSEncoder encoder(context);
  KeyGenerator keygen(context);
  const SecretKey &secret_key = keygen.secret_key();
  PublicKey public_key;
  keygen.create_public_key(public_key);
  RelinKeys relin_keys;
  keygen.create_relin_keys(relin_keys);
  GaloisKeys galois_keys;
  vector<int> rotation_steps = get_rotation_steps_fhe();
  if (!rotation_steps.empty())
    keygen.create_galois_keys(rotation_steps, galois_keys);
  Encryptor encryptor(context, public_key);
  Evaluator evaluator(context);
  Decryptor decryptor(context, secret_key);

  EncryptedArgs encrypted_inputs;
  CkksClearArgs plain_inputs;
  EncryptedArgs encrypted_outputs;
  EncodedArgs encoded_outputs;

  // CHEHAB timing convention (same as the BFV runtime): parse the inputs,
  // encode, encrypt and evaluate, single run.
  chrono::high_resolution_clock::time_point t = chrono::high_resolution_clock::now();
  CkksIoExample io = parse_ckks_io_file(is);
  prepare_ckks_inputs(encoder, encryptor, scale, io, encrypted_inputs, plain_inputs);
  fhe(
    encrypted_inputs, plain_inputs, encrypted_outputs, encoded_outputs, encoder, encryptor, evaluator, relin_keys,
    galois_keys, context);
  chrono::duration<double, milli> elapsed = chrono::high_resolution_clock::now() - t;

  // Evaluation only, comparable with the direct-SEAL reference: one warmup
  // run and `repeats` timed runs of fhe() on the same encrypted inputs.
  vector<double> eval_ms;
  for (size_t i = 0; i <= repeats; ++i)
  {
    EncryptedArgs run_encrypted_outputs;
    EncodedArgs run_encoded_outputs;
    auto start = chrono::high_resolution_clock::now();
    fhe(
      encrypted_inputs, plain_inputs, run_encrypted_outputs, run_encoded_outputs, encoder, encryptor, evaluator,
      relin_keys, galois_keys, context);
    chrono::duration<double, milli> run = chrono::high_resolution_clock::now() - start;
    if (i > 0)
      eval_ms.push_back(run.count());
  }

  CkksClearArgs obtained = decrypt_ckks_outputs(encoder, decryptor, encrypted_outputs, io.func_slot_count);
  bool ok = check_ckks_outputs(context, encrypted_inputs, encrypted_outputs, io.outputs, obtained, tolerance, cout);
  cout << "execution_time_(ms): " << elapsed.count() << "\n";
  if (!eval_ms.empty())
    cout << "fhe_eval_median_(ms): " << median(eval_ms) << " (1 warmup + " << repeats << " runs)\n";
  cout << (ok ? "PASS" : "FAIL") << " (tolerance " << tolerance << ")\n";
  return ok ? 0 : 1;
}
)";
}
} // namespace fheco::code_gen
