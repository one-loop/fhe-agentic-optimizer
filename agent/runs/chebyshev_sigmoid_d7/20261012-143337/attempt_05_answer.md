**Idea:** Attempt 4 is the fastest correct kernel so far (149.55 ms vs. 161.56 ms baseline), with four ciphertext multiplies and depth 5. I re-checked the alternatives. Degree 7 needs T2, T3, T4 and one product with T4, so four ciphertext multiplies is the floor for this split. Replacing the T4 split with a T3-based one still needs T4, so it doesn't reduce the count. The remaining plaintext multiplies are scalar-cheap, so I keep the attempt 4 kernel unchanged.

```cpp
// Chebyshev sigmoid degree 7 on [-4,4].
// Uses T_{4+j} = 2 T4 T_j - T_{4-j}:
//   c5 T5 + c7 T7 = T4 * (2 c5 z + 2 c7 T3) - c5 T3 - c7 z
// so only 4 ciphertext-ciphertext multiplies are needed.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;            // T1, depth 1

  Ciphertext zz = z * z;               // depth 2
  Ciphertext T2 = zz + zz - one;       // T2

  Ciphertext zT2 = z * T2;             // depth 3
  Ciphertext T3 = zT2 + zT2 - z;       // T3

  Ciphertext T2s = T2 * T2;            // depth 3
  Ciphertext T4 = T2s + T2s - one;     // T4

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  // Quotient Q = 2*c5*z + 2*c7*T3   (depth 4)
  Ciphertext c5z = z * c5;             // depth 2
  Ciphertext c7T3 = T3 * c7;           // depth 4
  Ciphertext Q = (c5z + c5z) + (c7T3 + c7T3);

  // Q * T4  (depth 5)
  Ciphertext P = Q * T4;

  // Remainder pieces
  Ciphertext c1z = z * c1;             // depth 2
  Ciphertext c7z = z * c7;             // depth 2
  Ciphertext A = c1z - c7z;

  Ciphertext c3T3 = T3 * c3;           // depth 4
  Ciphertext c5T3 = T3 * c5;           // depth 4
  Ciphertext B = c3T3 - c5T3;

  Ciphertext result = P + A;
  result = result + B;
  result = result + c0;
  result.set_output("y");
}
```