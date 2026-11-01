Attempt 1 at 100.78 ms is effectively the floor. With the coefficients only available as separate plaintexts, every variant I found that changed how they are combined either added plaintext operations or forced a scale alignment, and none measured faster. I don't expect this last attempt to beat it.

It keeps attempt 6's circuit:

- `square(z)`, which was marginally cheaper but not 3% faster;
- 4 ciphertext multiplications, 2 scalar multiplications, 6 levels and no scale alignments.

The only change is that the coefficient m is formed directly as 2w + c3 − 2c7 instead of going through `inner`. That makes it a distinct circuit, but it adds one plaintext addition, so it is not expected to be faster.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zs = square(z);
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7*T2
  Ciphertext v = u + u + c5;              // 2c7 T2 + c5
  Ciphertext w = t2 * v;                  // 2c7 T2^2 + c5 T2
  Ciphertext m = w + w + c3 - c7 - c7;    // c3 + 2(c7 T4 + c5 T2)
  Ciphertext q = (t2 - one) + t2;         // 2T2 - 1
  Ciphertext qm = q * m;
  Ciphertext s = qm + c1 - c5 - c7;
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```