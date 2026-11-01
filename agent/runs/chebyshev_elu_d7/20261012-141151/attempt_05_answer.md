**Idea:** The kernel is now 4 ciphertext-ciphertext multiplies (about 120 ms) plus 9 plaintext multiplies. The ciphertext multiplies are essentially minimal for degree 7 (T2, T3, T4, and the final T4·B product). The remaining lever is to avoid recomputing the shared plaintext products explicitly. T2·c6 is used in both A and B, so I compute it once and reuse it. The math is the same as the previous correct kernel. I expect only a small gain, since CHEHAB's CSE probably already merged the duplicate.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T3 = 2 z T2 - z
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;

  // T4 = 2 T2^2 - 1
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // Shared plaintext products
  Plaintext c6("c6");
  Ciphertext m6 = T2 * c6;      // c6 T2 (used in A with minus, in B with plus)
  Plaintext c7("c7");
  Ciphertext m7z = z * c7;      // c7 T1 (used in A with minus)
  Ciphertext m7T3 = T3 * c7;    // c7 T3 (used in B)
  Plaintext c5("c5");
  Ciphertext m5T3 = T3 * c5;    // c5 T3 (used in A with minus)
  Ciphertext m5z = z * c5;      // c5 T1 (used in B)

  // A = c0 + (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3
  Plaintext c1("c1");
  Ciphertext a1 = z * c1;
  Plaintext c2("c2");
  Ciphertext a2 = T2 * c2;
  Plaintext c3("c3");
  Ciphertext a3 = T3 * c3;
  Ciphertext A = a1 + a2;
  A = A + a3;
  A = A - m5T3;
  A = A - m6;
  A = A - m7z;

  // B = 2 (c5 T1 + c6 T2 + c7 T3) + c4
  Ciphertext Bp = m5z + m6;
  Bp = Bp + m7T3;
  Ciphertext B = Bp + Bp;
  Plaintext c4("c4");
  B = B + c4;

  // p = A + T4 * B
  Ciphertext P = T4 * B;
  Ciphertext result = A + P;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```