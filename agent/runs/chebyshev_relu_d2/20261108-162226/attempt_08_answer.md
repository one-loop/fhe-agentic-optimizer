I have no cheaper circuit left to try. Attempt 1 remains the best: it computes y = z·(c1 + 2·c2·z) + c0 − c2 with z = alpha·x, using one ciphertext-ciphertext multiplication and two plaintext multiplications. It measured 17.04 ms against the 24.07 ms baseline, and it was faster in all 5 alternating batches. I'm resubmitting it as the final kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext u = z * c2;
  Ciphertext v = u + u + c1;
  Ciphertext w = z * v;
  Ciphertext result = w + c0;
  result = result - c2;
  result.set_output("y");
}
```