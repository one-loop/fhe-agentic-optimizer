void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext A = ((zc3 + zc3) + c2) - ((zc5 + zc5) + c6);
  Ciphertext R = (zc7 + zc7) + c6;
  Ciphertext S = T2 * R;
  Ciphertext w = zc5 - zc7;
  Ciphertext v = w + S;
  Ciphertext inner = (v + v) + c4;
  Ciphertext hi = T4 * inner;
  Ciphertext TA = T2 * A;
  Ciphertext lin = (zc1 + zc5) - (zc3 + zc7);
  Ciphertext r = (hi + TA) + lin;
  r = r + c0;
  r.set_output("y");
}
