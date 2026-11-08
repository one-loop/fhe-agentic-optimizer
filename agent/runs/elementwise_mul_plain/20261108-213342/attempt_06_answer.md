I'm submitting the baseline again, because nothing cheaper exists. The function is one slot-wise product of `x0` with an arbitrary dense plaintext `p`. That needs exactly one ciphertext × plaintext multiply, plus the rescale CHEHAB adds to it. The baseline has only those two operations, at one level. Any different structure adds operations, so it would measure slower and never pass the 3% bar. I'm stopping here; the last three attempts can't improve on this.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```