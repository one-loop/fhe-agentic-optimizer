// Embedding lookup: each column's total is computed with a rotate-and-add tree
// (every slot holds the total). The three totals are then packed into slots
// 0,1,2 with a Horner scheme: out = ((S2*f)>>1 + S1*f)>>1 + S0*f, where f is
// first_slot. This uses 2 rotations for placement and 3 ciphertext-plaintext
// masks.
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> totals;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    totals.push_back(r);
  }

  Ciphertext acc = totals[2] * first_slot;
  acc = (acc >> 1) + (totals[1] * first_slot);
  acc = (acc >> 1) + (totals[0] * first_slot);
  acc.set_output("output");
}
