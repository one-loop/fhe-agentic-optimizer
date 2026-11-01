The scale mismatch came from adding `c4·T4` into a level-4 sum whose other terms had a different multiplication history. This version removes that collision by folding `c4` into the product term:

- c4·T4 + 2·T4·q = T4·(2q + c4). So the kernel computes P = T4·(q+q+c4), where the plaintext `c4` is added to the ciphertext q+q before the multiply.
- Each group of terms that share a plaintext-multiply pattern is combined only with other ciphertexts from the same pattern (c1·z with c7·z, c2·T2 with c6·T2, c3·T3 with c5·T3). The groups are then added at strictly increasing levels, so no two same-level sums have different histories.
- c0 is added last, as the baseline does.

Math check: y = c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3 + T4·(2(c5T1 + c6T2 + c7T3) + c4). This uses 4 ciphertext multiplies and depth 5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // Chebyshev basis
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // group A: (c1 - c7) T1   (level 2)
  Ciphertext A1 = z * c1;
  Ciphertext A2 = z * c7;
  Ciphertext A = A1 - A2;

  // group B: (c2 - c6) T2   (level 3)
  Ciphertext B1 = T2 * c2;
  Ciphertext B2 = T2 * c6;
  Ciphertext B = B1 - B2;

  // group C: (c3 - c5) T3   (level 4)
  Ciphertext C1 = T3 * c3;
  Ciphertext C2 = T3 * c5;
  Ciphertext C = C1 - C2;

  Ciphertext low = A + B;
  low = low + C;

  // q = c5 T1 + c6 T2 + c7 T3
  Ciphertext q1 = z * c5;
  Ciphertext q2 = T2 * c6;
  Ciphertext q3 = T3 * c7;
  Ciphertext q = q1 + q2;
  q = q + q3;

  // T4 * (2q + c4)  == 2 T4 q + c4 T4
  Ciphertext qq = q + q;
  qq = qq + c4;
  Ciphertext P = T4 * qq;

  Ciphertext result = P + low;
  result = result + c0;
  result.set_output("y");
}
```