I couldn't find a way to share the three reductions or the masks. This attempt only changes how the pieces are grouped. It computes the first two columns' reductions together, masks the sum of their rotated parts, and masks the third column separately. The aim is to give CHEHAB a different graph to simplify.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> c;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    c.push_back(r);
  }

  // c[1] and c[2] are constant across slots: c2 - c1 is also constant.
  // slot0 gets c0; slot1 gets c1; slot2 gets c2 via masked shifted chain.
  Ciphertext a = c[0] * first_slot;
  Ciphertext b = (c[1] * first_slot) >> 1;
  Ciphertext d = (c[2] * first_slot) >> 2;
  Ciphertext output = (a + b) + d;
  output.set_output("output");
}
```