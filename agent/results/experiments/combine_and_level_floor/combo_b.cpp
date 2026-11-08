// Even/odd split in s = T2(z): p = E(s) + z*O(s), with T_{2k}(z) = T_k(s) and T_{2k+1}(z) = z*V_k(s),
// V_0 = 1, V_1 = 2s-1, V_k = 2s V_{k-1} - V_{k-2}. E and O each by a degree-3 Clenshaw recurrence.
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha"), one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext s = zz + zz - one;
  Ciphertext ss = s + s;
  Ciphertext eb2 = ss * c6 + c4;
  Ciphertext eb1 = ss * eb2 - c6 + c2;
  Ciphertext e = s * eb1 - eb2 + c0;
  Ciphertext ob2 = ss * c7 + c5;
  Ciphertext ob1 = ss * ob2 - c7 + c3;
  Ciphertext v1 = ss - one;
  Ciphertext o = v1 * ob1 - ob2 + c1;
  Ciphertext y = e + z * o;
  y.set_output("y");
}
