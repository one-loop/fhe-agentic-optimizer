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
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(u);
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext s8 = s4 + s4;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u8 = u4 + u4;
  Ciphertext t4 = s8 - u8 + one;
  Ciphertext w = t2 * c6;
  Ciphertext v = w + w + c4;
  Ciphertext m = t4 * v;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - w;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}
