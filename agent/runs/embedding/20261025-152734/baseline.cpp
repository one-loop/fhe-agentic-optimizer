// Baseline: embedding lookup with 3 table columns, exactly like
// operators::embedding (benchmarks/extract_embedding_ckks, op embedding).
// indicator is an encrypted one-hot vector over slots(); table_i is the
// plaintext column i. Coordinate i = sum over all slots of indicator * table_i,
// computed by a rotate-and-add reduction (log2(slots()) rotations), which
// leaves the sum in every slot. The output holds coordinate i in slot i:
// each coordinate is masked to slot 0 with first_slot and rotated right by i.
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
