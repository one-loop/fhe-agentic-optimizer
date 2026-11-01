void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext v = zz * c2;
  Ciphertext w = v + c1;
  Ciphertext m = w * z;
  Ciphertext r = m + c0;
  r = r - c2;
  r.set_output("y");
}
