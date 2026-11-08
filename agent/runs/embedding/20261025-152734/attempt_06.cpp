void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> r;
  for (int i = 0; i < 3; ++i)
  {
    Ciphertext x = indicator * columns[i];
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      x += x << static_cast<int>(steps);
    r.push_back(x);
  }

  // r[i] is constant across slots (= total_i). Build the output with
  // differences so that only prefix masks are required.
  Ciphertext d01 = r[0] - r[1];
  Ciphertext d12 = r[1] - r[2];
  Ciphertext a = r[2] * first_slot;
  Ciphertext b = d12 * first_slot;
  Ciphertext c = d01 * first_slot;
  // out = c2*e2 + c1*e1 + c0*e0
  //     = a>>2 + (a + b)>>1 + (a + b + c)
  Ciphertext ab = a + b;
  Ciphertext abc = ab + c;
  Ciphertext out = abc + (ab >> 1) + (a >> 2);
  out.set_output("output");
}
