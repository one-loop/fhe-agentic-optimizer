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
  Ciphertext zz = square(z);
  Ciphertext t = zz + zz - one;
  Ciphertext t2 = square(t);
  Ciphertext s0 = t * c7;
  Ciphertext s2 = s0 + s0;
  Ciphertext s = s2 + c5 - c7;
  Ciphertext q = s * t2;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext tc3 = t * c3;
  Ciphertext tc5 = t * c5;
  Ciphertext dd = tc3 - tc5;
  Ciphertext p = dd + dd + q4 - s2 - s;
  p = p + c1 - c3;
  Ciphertext y = z * p;
  y.set_output("y");
}
