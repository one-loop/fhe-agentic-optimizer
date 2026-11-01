The previous kernel kept all six ct-ct products and was slower. This one uses an even/odd split in u = z². With u, u² and u³, each T_k or T_k/z is a small integer-coefficient polynomial in u, built from adds and `one`. That leaves 4 ct-ct multiplies instead of 6, with 6 coefficient multiplies plus the alpha one, and a depth of 6. The result is y = c0 + E(u) + z·(c1 + O(u)).

```cpp
Ciphertext mul_int(const Ciphertext &a, int n)
{
  Ciphertext r;
  bool h = false;
  Ciphertext p = a;
  while (n)
  {
    if (n & 1)
    {
      r = h ? r + p : p;
      h = true;
    }
    n >>= 1;
    if (n)
      p = p + p;
  }
  return r;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = square(u);
  Ciphertext u3 = u * u2;

  // even part
  Ciphertext T2 = mul_int(u, 2) - one;
  Ciphertext T4 = mul_int(u2, 8) - mul_int(u, 8) + one;
  Ciphertext T6 = mul_int(u3, 32) - mul_int(u2, 48) + mul_int(u, 18) - one;
  // odd part (divided by z)
  Ciphertext W3 = mul_int(u, 4) - one - one - one;
  Ciphertext W5 = mul_int(u2, 16) - mul_int(u, 20);
  W5 = W5 + one + one + one + one + one;
  Ciphertext W7 = mul_int(u3, 64) - mul_int(u2, 112) + mul_int(u, 56);
  W7 = W7 - one - one - one - one - one - one - one;

  Ciphertext E = T2 * c2;
  E = E + T4 * c4;
  E = E + T6 * c6;

  Ciphertext O = W3 * c3;
  O = O + W5 * c5;
  O = O + W7 * c7;
  O = O + c1;

  Ciphertext result = E + z * O;
  result = result + c0;
  result.set_output("y");
}
```