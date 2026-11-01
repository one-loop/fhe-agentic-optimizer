Idea: the previous kernel was only marginally faster than the baseline, so this keeps the 4-multiplication structure and changes how the work is arranged. T4 is no longer formed explicitly. The high part becomes 2·T2²·inner − inner, which saves a plaintext subtraction and an alignment on T4. The low-part and high-part terms that share a plaintext product are merged, so there are fewer scale alignments. I have not run this, so the gain is unconfirmed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = square(T2);

  Ciphertext U = T2 * c6;
  Ciphertext Q = (z * c5) + U + (T3 * c7);
  Ciphertext inner = (Q + Q) + c4;
  Ciphertext P = T2s * inner;
  Ciphertext hi = (P + P) - inner;

  Ciphertext lo = (z * c1) + (T2 * c2) + (T3 * c3);
  Ciphertext m = (z * c7) + U + (T3 * c5);
  Ciphertext r = (hi + lo) - m;
  r = r + c0;
  r.set_output("y");
}
```