I have no further circuit reduction to try, so my final answer is the best-confirmed kernel, attempt 5. It measured 166 ms against 242 ms for the baseline, and the alternating-batch comparison against attempt 4 showed it 6% faster (5/5 batches).

It cuts levels from 8 to 6 and ciphertext-ciphertext multiplications from 6 to 4. T5, T6 and T7 are folded into one product with T3, c3 is added into the same inner sum, and the c7 term is moved up one level so it never meets a same-level term of different scale history.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt = z * t2;
  Ciphertext t3 = zt + zt - z;
  Ciphertext tt = t2 * t2;
  Ciphertext t4 = tt + tt - one;

  Ciphertext u = t2 * c5 + t3 * c6;
  Ciphertext w = (t4 * c7) * one;
  Ciphertext inner = (u + u + c3) + (w + w);
  Ciphertext hi = t3 * inner;

  Ciphertext low = z * c1 + t2 * c2 + t4 * c4;
  low = low - z * c5;
  low = low - z * c7;
  low = low + c0;
  low = low - c6;

  Ciphertext y = hi + low;
  y.set_output("y");
}
```