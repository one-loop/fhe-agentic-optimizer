This is the last attempt, so it stays close to attempts 3 and 6, the fastest circuits measured. Those used 4 ciphertext multiplications (two as `square`), depth 5, T3 = z·(2T2 − 1) and a shared c6·T2.

The only change is the order of the high-part sum q. The T3 and T2 terms (levels 4 and 3) are combined first, and the level-2 T1 term is added last. That way the T1 term should be aligned directly to the final level of q instead of in two steps. This may remove one scale alignment, but I can't confirm that before it runs. The low part keeps the attempt-6 ordering, with c0 included.

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

  Ciphertext t1 = x * alpha;
  Ciphertext zz = square(t1);
  Ciphertext t2 = zz + zz - one;
  Ciphertext w = t2 + t2 - one;   // 2 T2 - 1
  Ciphertext t3 = t1 * w;         // T3 = z (2 T2 - 1)
  Ciphertext t22 = square(t2);
  Ciphertext t4 = t22 + t22 - one;

  Ciphertext m6 = t2 * c6;
  // q = c7 T3 + c6 T2 + c5 T1
  Ciphertext q = (t3 * c7 + m6) + t1 * c5;
  Ciphertext s = q + q + c4;
  Ciphertext top = t4 * s;

  // low = (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3 + c0
  Ciphertext l1 = t1 * c1 - t1 * c7;
  Ciphertext l2 = t2 * c2 - m6;
  Ciphertext l3 = t3 * c3 - t3 * c5;
  Ciphertext low = l1 + l2 + l3 + c0;

  Ciphertext y = top + low;
  y.set_output("y");
}
```