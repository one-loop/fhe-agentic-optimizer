Idea: this is the last attempt, and the identical kernel has measured 353 to 356 ms, so the differences are run-to-run noise. I resubmit attempt 1's kernel, the best measured one. It needs 4 ct-ct multiplies and depth 5, against 6 and depth 8 in the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext s2 = T2 * T2;
  Ciphertext T4 = s2 + s2 - one;

  Ciphertext a5 = z * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext inner = a5 + a6 + a7;
  Ciphertext Q = inner + inner + c4;
  Ciphertext P = T4 * Q;

  Ciphertext r = P + c0;
  r = r + z * c1 - z * c7;
  r = r + T2 * c2 - T2 * c6;
  r = r + T3 * c3 - T3 * c5;
  r.set_output("y");
}
```