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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext u = square(z);            // level 2
  Ciphertext t2 = u + u - one;         // level 2
  Ciphertext s = square(t2);           // level 3 (T4 = 2s - 1)

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext sw = s * w;               // level 4
  Ciphertext high = sw + sw;           // T4*w + w

  Ciphertext lin = z * c1 + c0;        // level 2
  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext low = (c2t2 - c6t2 - w) + lin; // level 3

  Ciphertext result = low + high;
  result.set_output("y");
}
