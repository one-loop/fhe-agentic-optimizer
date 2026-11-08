Idea: this is the same dataflow as the baseline, restructured only to change where the masking happens. Each column is folded with 13 rotate-and-add steps. Each half-sum is then masked with `first_slot`, and the two halves are combined with the right-shifts into the output slots. I could not find a way to share the reduction rotations across columns, so this is unlikely to beat the baseline. It is a test of a different operation mix.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  int half = static_cast<int>(slots() >> 1);
  Ciphertext output;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (int steps = 1; steps < half; steps <<= 1)
      r += r >> steps;
    Ciphertext a = r * first_slot;
    Ciphertext b = (r << half) * first_slot;
    Ciphertext m = a + b;
    if (i > 0)
      m = m >> i;
    if (i == 0)
      output = m;
    else
      output += m;
  }
  output.set_output("output");
}
```