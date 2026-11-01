Attempt 2 cut the ciphertext multiplications from 6 to 4, but scale alignments ate much of the gain. This attempt removes one plaintext multiplication and one alignment.

The change is in the high part. Instead of computing 2·T4·q + c4·T4, it computes T4·(2q + c4). The coefficient c4 then enters as a cheap plaintext add to q before the single ciphertext multiplication by T4. This drops the separate T4·c4 multiply and the alignment between the level-4 and level-5 terms.

The rest of the circuit is the same as attempt 2. This kernel has not been run yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = T2 * T2;
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