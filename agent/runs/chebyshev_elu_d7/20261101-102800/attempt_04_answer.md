Starting from attempt 3, this changes how the T4 product is formed. T4 is replaced by T4' = 2·T2² = T4 + 1, which drops one plaintext-constant add (encode). The top term is rewritten as T4·s = T4'·s − s. The extra `− s` is a cheap ciphertext subtraction of a level-4 value, below the level-5 product.

Everything else stays the same: T3 = z·(2T2 − 1), 4 ciphertext multiplications and depth 5.

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
  Ciphertext t4p = t22 + t22;     // T4 + 1

  // q = c5 T1 + c6 T2 + c7 T3
  Ciphertext q = t1 * c5 + t2 * c6 + t3 * c7;
  Ciphertext s = q + q + c4;
  Ciphertext top = t4p * s - s;   // = T4 * s

  // low = (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3 + c0
  Ciphertext l1 = t1 * c1 - t1 * c7;
  Ciphertext l2 = t2 * c2 - t2 * c6;
  Ciphertext l3 = t3 * c3 - t3 * c5;
  Ciphertext low = l1 + l2 + l3 + c0;

  Ciphertext y = top + low;
  y.set_output("y");
}
```