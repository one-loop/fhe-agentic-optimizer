void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Ciphertext out;
  for (int i = 2; i >= 0; --i)
  {
    Plaintext col("table_" + to_string(i));
    Ciphertext x = indicator * col;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      x += x << static_cast<int>(steps);
    Ciphertext m = x * first_slot;
    if (i == 2)
      out = m;
    else
      out = (out >> 1) + m;
  }
  out.set_output("output");
}
