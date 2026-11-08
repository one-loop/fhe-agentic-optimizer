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
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = t2 * t2;
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a7 = t4 * t3;
  Ciphertext t7 = a7 + a7 - z;
  Ciphertext p3 = (z * c3) * t2;
  Ciphertext p5 = (t2 * c5) * t3;
  Ciphertext zt = z * c1 - z * c3 - z * c5;
  Ciphertext r = zt + (p3 + p3);
  r = r + (p5 + p5);
  r = r + t7 * c7;
  r.set_output("y");
}
