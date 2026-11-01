This attempt uses a deeper Horner form that needs fewer scale joins and fewer plaintext multiplications. With w = 4z²:

y = c0 + z·[c1 + c3·(w − 3) + c5·((w − 2)(w − 3) − 1)]

This expands correctly, since z(w − 3) = T3 and z(w² − 5w + 5) = T5. The integer offsets come from adding or subtracting the `one` plaintext:

- T2 = 2z² − one, then m = 2·T2 = w − 2 and n = m − one = w − 3.
- t5 = m·n − one.

The bracket uses plaintext adds for c1 and c0. The result is 3 ct-ct multiplications (z², m·n and z·B) and only 3 scalar plaintext multiplications (alpha, c3, c5), down from 4. There is only one mixed-level join, between n·c3 and t5·c5. The cost is one extra level (depth 5), but the expensive operations sit lower in the chain.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = (z2 + z2) - one;   // 2z^2 - 1
  Ciphertext m = t2 + t2;            // 4z^2 - 2
  Ciphertext n = m - one;            // 4z^2 - 3
  Ciphertext t5 = m * n - one;       // 16z^4 - 20z^2 + 5

  Ciphertext B = (n * c3 + t5 * c5) + c1;
  Ciphertext result = z * B + c0;
  result.set_output("y");
}
```