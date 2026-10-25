void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext m = t2 * t4;
  Ciphertext d = t4 - t2;
  Ciphertext e = m + m - t4;
  Ciphertext g3 = t2 + t2 - one;
  Ciphertext g5 = d + d + one;
  Ciphertext g7 = e + e - one;
  Ciphertext q = g3 * c3;
  q = q + c1;
  q = q + g5 * c5;
  q = q + g7 * c7;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
