Idea: factor the even part as a cubic in t = T2, so T4 and T6 are never formed separately.

- T4 = 2t² − 1 and T6 = t(4t² − 3).
- y = c0 + c1·z + c4·(2s − 1) + t·(c2 + c6·(4s − 3)), with s = t².
- This needs 3 ct-ct multiplications (u, s, and the final product by t) and 4 plaintext multiplications (alpha, c1, c4, c6), one fewer than before. Depth stays at 5.

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
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext s2 = s + s;
  Ciphertext t4 = s2 - one;
  Ciphertext h = s2 + s2 - one - one - one;
  Ciphertext inner = h * c6 + c2;
  Ciphertext result = t * inner + t4 * c4 + z * c1 + c0;
  result.set_output("y");
}
```