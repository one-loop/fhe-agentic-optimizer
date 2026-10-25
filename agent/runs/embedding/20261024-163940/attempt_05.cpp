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
  Ciphertext out = r[0] * first_slot;
  out += (r[1] * first_slot) >> 1;
  out += (r[2] * first_slot) >> 2;
  out.set_output("output");
}
