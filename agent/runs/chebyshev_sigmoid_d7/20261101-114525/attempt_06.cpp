void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zs = square(z);
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7*T2
  Ciphertext v = u + u + c5;              // 2c7 T2 + c5
  Ciphertext w = t2 * v;                  // 2c7 T2^2 + c5 T2
  Ciphertext inner = w - c7;              // c7 T4 + c5 T2
  Ciphertext m = inner + inner + c3;      // c3 + 2 inner
  Ciphertext q = (t2 - one) + t2;         // 2T2 - 1
  Ciphertext qm = q * m;
  Ciphertext s = qm + c1 - c5 - c7;
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
