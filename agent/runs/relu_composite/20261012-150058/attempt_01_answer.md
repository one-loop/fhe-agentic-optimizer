The baseline already needs depth 3 per stage (10 levels total, with the final `x*h` using the last one), and I don't see a degree-7 evaluation that gets below 5 ciphertext multiplications per stage at that depth. This version keeps depth 3 per stage and restructures the sum as `p = t^4·A + B`, with `A = c5·t + (c7·t)·t^2` and `B = (c3·t)·t^2 + c1·t`. The ciphertext-ciphertext count stays at 5 per stage, but more of the coefficient multiplications happen at shallower levels, where they are cheaper, and the additions should need fewer alignments. The expected gain is modest.

```cpp
// Per stage (depth 3):
//   t2 = t^2, t4 = t2^2
//   A  = c5*t + (c7*t)*t^2      (= c5 t + c7 t^3)
//   B  = (c3*t)*t^2 + c1*t      (= c3 t^3 + c1 t)
//   p  = t4*A + B               (= c1 t + c3 t^3 + c5 t^5 + c7 t^7)
// Ciphertext multiplies: t^2, t^4, (c7 t)t^2, (c3 t)t^2, A*t^4 (5 total, as in baseline),
// but coefficient multiplies happen at shallower levels and the adds align more cheaply.

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