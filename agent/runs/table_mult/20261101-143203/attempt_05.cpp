void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> t;
  for (int i = 0; i < 3; ++i)
    t.emplace_back("table_" + to_string(i));

  vector<Ciphertext> p;
  for (int i = 0; i < 3; ++i)
    p.push_back(indicator * t[i]);

  Ciphertext A = p[0] * t[1];
  Ciphertext B = p[1] * t[0];
  Ciphertext z = A - B; // mathematically zero, at level 2

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i];
    int steps = slots() >> 1;
    for (int k = 0; k < 2 && steps > 0; ++k, steps >>= 1)
      r += r << steps;
    r = r + z;
    for (; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
