void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext u = (zz * c2) + c1;
  Ciphertext p = z * u;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
