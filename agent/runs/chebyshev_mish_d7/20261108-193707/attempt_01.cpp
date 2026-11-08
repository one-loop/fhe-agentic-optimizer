void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;                 // level 1
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;             // level 2
  Ciphertext zt2 = t1 * t2;
  Ciphertext t3 = zt2 + zt2 - t1;            // level 3
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;           // level 3

  Ciphertext c6t2 = t2 * c6;
  Ciphertext a = t1 * c5 + c6t2 + t3 * c7;   // up to level 4
  Ciphertext b = t3 * c5 + c6t2 + t1 * c7;

  Ciphertext w = t4 * a;                     // level 5
  Ciphertext high = w + w - b;

  Ciphertext low = t1 * c1 + t2 * c2 + t3 * c3 + t4 * c4;
  Ciphertext result = low + high + c0;
  result.set_output("y");
}
