void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext p4 = square(t2);
  Ciphertext t4 = p4 + p4 - one;

  Ciphertext a1 = z * c1 - z * c7;
  Ciphertext a2 = t2 * c2 - t2 * c6;
  Ciphertext a3 = t3 * c3 - t3 * c5;

  Ciphertext s = z * c5;
  Ciphertext s2 = t2 * c6;
  Ciphertext s3 = t3 * c7;
  Ciphertext lowR = s + s2;
  Ciphertext r = lowR + s3;
  Ciphertext R = r + r + c4;

  Ciphertext top = t4 * R;
  Ciphertext low = a1 + a2;
  low = low + a3;
  Ciphertext y = low + top;
  y = y + c0;
  y.set_output("y");
}
