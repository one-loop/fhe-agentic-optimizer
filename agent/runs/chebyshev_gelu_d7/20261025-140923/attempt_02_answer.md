Idea: write the even part as a polynomial in u = T2(z) = 2z² − 1, using T4 = 2u² − 1 and T6 = 4u³ − 3u. Then:

y = c0 − c4 + c1·z + c2·u − 3·c6·u + 2·u²·(c4 + 2·c6·u)

Setting a = c6·u, this becomes:
- b = c4 + 2a
- y = c0 − c4 + c1·z + c2·u − 3a + 2·u²·b

This needs 3 ct-ct multiplications (two squares and one product), as before. It cuts plaintext multiplications from 5 to 3, because `a` is reused, and the depth drops to 4. No plaintext-plaintext arithmetic is needed, since every constant is added to a ciphertext term.

```cpp
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
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext u = s + s - one;
  Ciphertext q = square(u);
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext m = q * b;
  Ciphertext r = z * c1 + u * c2;
  r = r - (a2 + a);
  r = r + (m + m);
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```