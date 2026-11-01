**Idea:** Attempts 1, 3 and 4 are the same kernel within noise (136 to 137 ms), and attempt 2 was slightly slower. I checked whether the four ciphertext multiplications can be reduced to three. A degree-7 polynomial needs a product chain of length 4 to produce its z⁷ term, so I don't see a three-multiplication schedule here. Removing the `alpha` multiply also doesn't work, because `1/8` and `1/16` can't be expressed without plaintext-plaintext arithmetic. I'm therefore resubmitting the attempt-3 kernel, the fastest measured at 136.14 ms.

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
  Plaintext c7("c7");

  // z = x / 4 in [-1, 1]  (depth 1)
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1  (depth 2)
  Ciphertext u = z * z;
  Ciphertext T2 = u + u - one;

  // T3 = 2 z T2 - z  (depth 3)
  Ciphertext p3 = z * T2;
  Ciphertext T3 = p3 + p3 - z;

  // T5 = 2 T2 T3 - z  (depth 4)
  Ciphertext p5 = T2 * T3;
  Ciphertext T5 = p5 + p5 - z;

  // T7 = 2 T2 T5 - T3  (depth 5)
  Ciphertext p7 = T2 * T5;
  Ciphertext T7 = p7 + p7 - T3;

  // y = c0 + c1 T1 + c3 T3 + c5 T5 + c7 T7
  Ciphertext y = z * c1;
  y = y + T3 * c3;
  y = y + T5 * c5;
  y = y + T7 * c7;
  y = y + c0;
  y.set_output("y");
}
```