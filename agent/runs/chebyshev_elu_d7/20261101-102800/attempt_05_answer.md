This keeps the structure of attempt 3: 4 ciphertext multiplications, depth 5, and T3 = z·(2T2 − 1). The c6·T2 product is shared between the high and low parts.

The change is only in how the final sums are ordered:

- **c0 added last:** it now goes onto the final level-5 sum, so its encoding is the cheapest one. Before, it was added at level 4.
- **Low part summed deepest-first:** the terms go in the order T3, T2, T1. That way the level-2 and level-3 terms get aligned into an existing sum once, not twice.

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
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;
  Ciphertext w = t2 + t2 - one;   // 2 T2 - 1
  Ciphertext t3 = t1 * w;         // T3 = z (2 T2 - 1)
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;

  Ciphertext m6 = t2 * c6;
  // q = c5 T1 + c6 T2 + c7 T3
  Ciphertext q = t3 * c7 + m6 + t1 * c5;
  Ciphertext s = q + q + c4;
  Ciphertext top = t4 * s;

  // low = (c3 - c5) T3 + (c2 - c6) T2 + (c1 - c7) T1
  Ciphertext l3 = t3 * c3 - t3 * c5;
  Ciphertext l2 = t2 * c2 - m6;
  Ciphertext l1 = t1 * c1 - t1 * c7;
  Ciphertext low = l3 + l2 + l1;

  Ciphertext y = top + low + c0;
  y.set_output("y");
}
```