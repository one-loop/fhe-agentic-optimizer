Attempt 4 removed one scale alignment and was about 2% faster, just short of the 3% needed to count. This attempt keeps that circuit and writes the two self-products z·z and T2·T2 as `square()`. SEAL's squaring needs fewer polynomial multiplications than a general ciphertext-ciphertext multiply, so it should be slightly cheaper. It also changes the operation counts, so this is a distinct circuit.

Everything else is attempt 4:
- **T3:** built as z·(2·T2 − one), which avoids the alignment.
- **High part:** T4·(2q + c4), with q = c5·z + c6·T2 + c7·T3.
- **Low part:** (c1 − c7)·z + (c2 − c6)·T2 + (c3 − c5)·T3 + c0.

This has not been run yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext T2 = zz + zz - one;
  Ciphertext W = T2 + T2 - one;
  Ciphertext T3 = z * W;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext q = z * c5 + c6T2 + T3 * c7;
  Ciphertext q2 = (q + q) + c4;
  Ciphertext high = T4 * q2;

  Ciphertext low = (z * c1 - z * c7) + (T2 * c2 - c6T2) + (T3 * c3 - T3 * c5);
  Ciphertext result = high + low + c0;
  result.set_output("y");
}
```