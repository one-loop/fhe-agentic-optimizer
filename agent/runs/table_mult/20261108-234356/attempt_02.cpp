Ciphertext reduce_all(Ciphertext r)
{
  for (int steps = slots() >> 1; steps > 0; steps >>= 1)
    r += r << steps;
  return r;
}

void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext t0("table_0");
  Plaintext t1("table_1");
  Plaintext t2("table_2");
  Ciphertext p0 = indicator * t0;
  Ciphertext p1 = indicator * t1;
  Ciphertext p2 = indicator * t2;
  Ciphertext o0 = reduce_all(p0);
  Ciphertext o1 = reduce_all(p1);
  Ciphertext s = reduce_all(p0 + p1 + p2);
  Ciphertext o2 = s - o0 - o1;
  o0.set_output("output_0");
  o1.set_output("output_1");
  o2.set_output("output_2");
}
