void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext A = ((zc3 + zc3) + c2) - ((zc5 + zc5) + c6);
  Ciphertext R = (zc7 + zc7) + c6;
  Ciphertext S = T2 * R;
  Ciphertext w = zc5 - zc7;
  Ciphertext g2 = (w + w) + c4;
  Ciphertext g = g2 + g2;
  Ciphertext S4 = (S + S) + (S + S);
  Ciphertext P = T2 * (g + S4);
  Ciphertext U = A - (R + R);
  Ciphertext F = T2 * (U + P);
  Ciphertext lin = (zc1 + zc5) - (zc3 + zc7);
  Ciphertext rem = ((lin - g2) + c0);
  Ciphertext r = rem + F;
  r.set_output("y");
}
