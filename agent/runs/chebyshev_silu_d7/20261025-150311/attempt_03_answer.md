Idea: merge the c4 and c6 terms into a single ct-ct product. Since T6 = T2·(2·T4 − 1), the sum c4·T4 + c6·T6 equals T4·(c4 + 2·c6·T2) − c6·T2. The factor w = c4 + 2·c6·T2 sits at level 3 and multiplies T4 once, so this saves a plaintext multiplication. The other terms stay at level 3 or below, so they meet the level-4 product at a different level and no scale mismatch arises.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c6;
  Ciphertext w = a + a + c4;
  Ciphertext prod = t4 * w;
  Ciphertext low = t2 * c2 - a;
  low = low + z * c1;
  low = low + c0;
  Ciphertext result = prod + low;
  result.set_output("y");
}
```