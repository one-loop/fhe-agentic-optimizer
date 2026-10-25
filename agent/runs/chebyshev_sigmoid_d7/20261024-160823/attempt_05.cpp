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
  Ciphertext mp = (t2 * c7) * t4;
  Ciphertext mp2 = mp + mp;
  Ciphertext mp4 = mp2 + mp2;
  Ciphertext d = t4 - t2;
  Ciphertext g3 = t2 + t2 - one;
  Ciphertext g5 = d + d + one;
  Ciphertext h7 = t4 + t4 + one;
  Ciphertext q = g3 * c3;
  q = q + g5 * c5;
  q = q - h7 * c7;
  q = q + mp4;
  q = q + c1;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
