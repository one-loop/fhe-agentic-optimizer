Attempt 4 remains the best. The Chebyshev split of attempt 5 used too many plaintext multiplies at the expensive top levels. I return to Clenshaw and change only the first step. `b5` becomes `sq·c7 + zz·c6 + c5 − c7` with `sq = square(zz)`, and `sq` is also reused in the next step for `b4`. This changes the circuit, and I have no estimate of whether it is faster than attempt 4.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext sq = square(zz);
  Ciphertext b6 = zz * c7 + c6;
  Ciphertext b5 = sq * c7 + zz * c6 + c5 - c7;
  Ciphertext b4 = zz * b5 + c4 - b6;
  Ciphertext b3 = zz * b4 + c3 - b5;
  Ciphertext b2 = zz * b3 + c2 - b4;
  Ciphertext b1 = zz * b2 + c1 - b3;
  Ciphertext y = z * b1 + c0 - b2;
  y.set_output("y");
}
```