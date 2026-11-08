Idea: use Horner-style nesting of the masking and shifting, so the right-shifts happen on already-combined masked values. This is the same arithmetic as before. It is mainly a probe to see how CHEHAB's rewrites treat the combine stage. I found no way to share the reduction rotations across columns without extra masks.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> m;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = indicator * columns[i];
    for (size_t steps = 1; steps < slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    m.push_back(r * first_slot);
  }
  Ciphertext acc = m[2] >> 1;
  acc += m[1];
  acc = acc >> 1;
  acc += m[0];
  acc.set_output("output");
}
```