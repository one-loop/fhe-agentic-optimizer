void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;          // T2
  Ciphertext s = w * w;
  Ciphertext s2 = s + s;
  Ciphertext r = w * c6;
  Ciphertext r2 = r + r;
  Ciphertext v = r2 + c4;
  Ciphertext t = s2 * v;
  Ciphertext r3 = r2 + r;
  Ciphertext t2 = w * c2;
  Ciphertext t1 = z * c1;
  Ciphertext low = t2 - r3;
  low = low + t1;
  low = low + c0;
  low = low - c4;
  Ciphertext result = t + low;
  result.set_output("y");
}
