void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> t;
  for (int i = 0; i < 3; ++i)
    t.emplace_back("table_" + to_string(i));

  vector<Ciphertext> p;
  for (int i = 0; i < 3; ++i)
    p.push_back(indicator * t[i]);

  // Two chains with the same multiset of table factors in different order:
  // A: t0,t1,t2,t0,t1,t2,t0,t1   B: t1,t0,t2,t1,t0,t2,t1,t0
  int seqA[8] = {0, 1, 2, 0, 1, 2, 0, 1};
  int seqB[8] = {1, 0, 2, 1, 0, 2, 1, 0};
  Ciphertext A = p[seqA[0]];
  Ciphertext B = p[seqB[0]];
  for (int k = 1; k < 8; ++k)
  {
    A = A * t[seqA[k]];
    B = B * t[seqB[k]];
  }
  Ciphertext z = A - B; // mathematically zero, at deep level

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i] + z;
    for (int steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
