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
  Ciphertext t = zz + zz - one;
  Ciphertext t2 = t * t;
  Ciphertext s0 = t * c7;
  Ciphertext s = s0 + s0;
  s = s + c5 - c7;
  s = s + s;
  s = s + s;
  Ciphertext q = s * t2;
  Ciphertext tc3 = t * c3;
  Ciphertext tc5 = t * c5;
  Ciphertext d = tc3 - tc5 - s0 - s0;
  Ciphertext lin = d + d;
  lin = lin + c1 - c3 - c5 + c7;
  Ciphertext p = lin + q;
  Ciphertext y = z * p;
  y.set_output("y");
}
