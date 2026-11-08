Idea: use Clenshaw's recurrence. With b7 = c7, b6 = c6 + 2z·c7, then b_k = c_k + 2z·b_{k+1} − b_{k+2}, and y = c0 + z·b1 − b2. The coefficients enter as plaintext adds rather than as multiplications, so the eight coefficient multiplies of the baseline (and the rescales and scale alignments that go with them) disappear. That leaves 6 ct-ct multiplies and only 2 plaintext multiplies: `x*alpha` and `c7*2z`. The terms that meet in each `+` or `-` sit at different levels, which avoids the scale-mismatch error.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext b6 = z2 * c7 + c6;
  Ciphertext p5 = z2 * b6;
  Ciphertext b5 = p5 + c5 - z2 * c7 * Plaintext("one");
  (void)b5;
  Ciphertext r;
  r = b5;
  r.set_output("y");
}
```

Wait, that draft is wrong: it has a bogus term and uses `c7` as a ciphertext. Here is the corrected kernel. `b5 = c5 + 2z·b6 − c7` needs c7 as a plaintext subtraction, which is allowed because a ciphertext operand is present:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext b6 = z2 * c7 + c6;
  Ciphertext b5 = z2 * b6 + c5 - c7;
  Ciphertext b4 = z2 * b5 + c4 - b6;
  Ciphertext b3 = z2 * b4 + c3 - b5;
  Ciphertext b2 = z2 * b3 + c2 - b4;
  Ciphertext b1 = z2 * b2 + c1 - b3;
  Ciphertext y = z * b1 + c0 - b2;
  y.set_output("y");
}
```