I'm not going to spend the remaining attempts on this. Attempt 6 measured 318.30 ms, the same kernel as attempts 1, 3 and 4 within noise, so the variation between runs (317.9–318.5 ms) is larger than any difference between the variants I've tried.

The best measured kernel is attempt 3 at 317.94 ms, and it is the kernel below. It is only about 0.2% faster than the 318.50 ms baseline.

The limit I see is structural. Under the 10-level budget, each degree-7 stage must have depth 3, and that requires 5 ciphertext multiplies per stage. The final `x*h` is unavoidable, so 16 ciphertext multiplies is the floor for this formulation. I tried the variants I could find that reduce plaintext multiplies or alignments, and CHEHAB's normalization gave the same graph or a slower one (attempt 2, 327 ms).

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