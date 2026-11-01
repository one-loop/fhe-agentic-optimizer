Four attempts have now converged on the same operation counts, so I don't see a further structural change that would lower the time. Under the depth budget, each degree-7 stage needs 5 ciphertext multiplies, and the final `x*h` is unavoidable. I'm resubmitting the best kernel.

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