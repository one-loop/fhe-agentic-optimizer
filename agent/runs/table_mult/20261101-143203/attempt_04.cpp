void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> t;
  for (int i = 0; i < 3; ++i)
    t.emplace_back("table_" + to_string(i));

  vector<Ciphertext> p;
  for (int i = 0; i < 3; ++i)
    p.push_back(indicator * t[i]);

  Ciphertext A = p[0] * t[2];
  Ciphertext B = p[2] * t[0];
  Ciphertext z = A - B; // mathematically zero, at level 2

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i] + z;
    for (int steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
