void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext T2 = zz + zz - one;
  Ciphertext W = T2 + T2 - one;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext E = (c6T2 + c6T2) + c4;
  Ciphertext O = W * c7 + c5;
  Ciphertext z2 = z + z;
  Ciphertext zO2 = z2 * O;
  Ciphertext high = T4 * (E + zO2);

  Ciphertext R = ((W * c3 - W * c5) + c1) - c7;
  Ciphertext odd = z * R;

  Ciphertext even_low = (T2 * c2 - c6T2) + c0;
  Ciphertext result = high + odd + even_low;
  result.set_output("y");
}
