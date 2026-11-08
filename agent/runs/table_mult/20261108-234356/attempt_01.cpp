void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext t0("table_0");
  Plaintext t1("table_1");
  Plaintext t2("table_2");
  vector<Ciphertext> prods;
  prods.push_back(indicator * t0);
  prods.push_back(indicator * t1);
  prods.push_back(indicator * t2);
  int n = slots();
  for (size_t i = 0; i < prods.size(); ++i)
  {
    Ciphertext r = prods[i];
    for (int steps = 1; steps < n; steps <<= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
