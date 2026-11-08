void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext t0("table_0"), t1("table_1"), t2("table_2");

  Ciphertext p0 = indicator * t0;
  Ciphertext p1 = indicator * t1;
  Ciphertext p2 = indicator * t2;
  Ciphertext s12 = p1 + p2;

  auto fold = [](Ciphertext r) {
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    return r;
  };

  Ciphertext r0 = fold(p0);
  Ciphertext r1 = fold(p1);
  Ciphertext r12 = fold(s12);

  Ciphertext m0 = r0 * first_slot;
  Ciphertext m1 = r1 * first_slot;
  Ciphertext m12 = r12 * first_slot;
  Ciphertext m2 = m12 - m1;

  Ciphertext acc = m2 >> 1;
  acc += m1;
  acc = acc >> 1;
  acc += m0;
  acc.set_output("output");
}
