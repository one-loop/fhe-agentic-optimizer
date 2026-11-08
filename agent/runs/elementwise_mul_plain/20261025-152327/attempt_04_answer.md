I still can't find a circuit with fewer operations than the baseline, and three identical resubmissions have shown that repeating it gains nothing. The computation is a single ct-pt multiply and one rescale. Every other form I can write under the CKKS rules adds a multiply, a rotation or an alignment, so it would be slower. I'm leaving the baseline kernel as my answer, and it should stay the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```