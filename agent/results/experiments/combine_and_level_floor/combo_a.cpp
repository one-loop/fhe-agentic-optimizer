// p = R + T4*Q (baby-step giant-step split), R and Q each by a degree-3 Clenshaw recurrence.
// Q = c4 + 2*(c5 T1 + c6 T2 + c7 T3); R = c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3.
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha"), one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext zc7 = z * c7;
  Ciphertext s2 = zc7 + zc7 + c6;
  Ciphertext m1 = z * s2;
  Ciphertext s1 = m1 + m1 - c7 + c5;
  Ciphertext s = z * s1 - s2;
  Ciphertext q = s + s + c4;
  Ciphertext zd3 = z * c3 - z * c5;
  Ciphertext r2 = zd3 + zd3 + c2 - c6;
  Ciphertext n1 = z * r2;
  Ciphertext r1 = n1 + n1 - c3 + c5 + c1 - c7;
  Ciphertext r = z * r1 - r2 + c0;
  Ciphertext y = r + t4 * q;
  y.set_output("y");
}
