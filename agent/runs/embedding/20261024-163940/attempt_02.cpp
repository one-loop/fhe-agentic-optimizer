void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Ciphertext> r;
  for (int i = 0; i < 3; ++i)
  {
    Plaintext col("table_" + to_string(i));
    Ciphertext x = indicator * col;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      x += x << static_cast<int>(steps);
    r.push_back(x);
  }
  Ciphertext y = r[2] * first_slot;
  y = y >> 1;
  y += r[1] * first_slot;
  y = y >> 1;
  y += r[0] * first_slot;
  y.set_output("output");
}
