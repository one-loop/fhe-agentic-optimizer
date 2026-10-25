void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext p = m * (z + z);
  Ciphertext l = x * c1;
  Ciphertext la = l * alpha;
  Ciphertext y = ((p + la) + c0) - c2;
  y.set_output("y");
}
