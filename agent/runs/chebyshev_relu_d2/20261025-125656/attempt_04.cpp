void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext v = z * c2;
  Ciphertext m = v * zz;
  Ciphertext l = z * c1;
  Ciphertext r = m + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
