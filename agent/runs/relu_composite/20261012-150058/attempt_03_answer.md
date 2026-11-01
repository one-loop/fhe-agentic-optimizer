Attempt 2 was slower (327 ms), so I'm returning to the attempt 1 kernel, the best so far at 317.94 ms. I also checked whether the ciphertext multiply count can drop below 5 per stage at depth 3. The alternatives I tried either reach depth 4 per stage, which exceeds the 10-level budget with the final multiply, or leave the count unchanged. The remaining gains are small reductions in plaintext multiplies and alignment operations, so this kernel is essentially at the practical limit for this structure.

```cpp
// Per stage (depth 3):
//   t2 = t^2, t4 = t2^2
//   A  = c5*t + (c7*t)*t^2      (= c5 t + c7 t^3)
//   B  = (c3*t)*t^2 + c1*t      (= c3 t^3 + c1 t)
//   p  = t4*A + B               (= c1 t + c3 t^3 + c5 t^5 + c7 t^7)
// 5 ciphertext multiplies per stage at depth 3; final y = x*(t3 + half).

Ciphertext odd_stage(const Ciphertext &t, int s)
{
  Plaintext c1("s" + to_string(s) + "c1");
  Plaintext c3("s" + to_string(s) + "c3");
  Plaintext c5("s" + to_string(s) + "c5");
  Plaintext c7("s" + to_string(s) + "c7");

  Ciphertext t2 = t * t;
  Ciphertext t4 = t2 * t2;
  Ciphertext A = t * c5 + (t * c7) * t2;
  Ciphertext B = (t * c3) * t2 + t * c1;
  return t4 * A + B;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = odd_stage(t, stage);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```