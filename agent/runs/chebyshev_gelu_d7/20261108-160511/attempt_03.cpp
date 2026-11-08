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
  Ciphertext s4 = s2 + s2;
  Ciphertext q = s4 - one - one - one; // 4w^2 - 3
  Ciphertext wq = w * q;
  Ciphertext t6 = wq * c6;
  Ciphertext t4 = (s2 - one) * c4;
  Ciphertext t2 = w * c2;
  Ciphertext t1 = z * c1;
  Ciphertext result = t6 + t4;
  result = result + t2;
  result = result + t1;
  result = result + c0;
  result.set_output("y");
}
