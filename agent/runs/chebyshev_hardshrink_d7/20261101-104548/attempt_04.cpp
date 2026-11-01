void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;          // T2
  Ciphertext q = s + s - one;            // 2T2 - 1

  Ciphertext e = s * c7;
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext g = e4 + c5 + c5;           // 4 c7 T2 + 2 c5
  Ciphertext d = s * c3;

  Ciphertext h = q * s;
  Ciphertext hg = h * g;

  Ciphertext m = hg + d + d - e4 - c3 + c7 + c1 - c5;
  Ciphertext result = z * m;
  result.set_output("y");
}
