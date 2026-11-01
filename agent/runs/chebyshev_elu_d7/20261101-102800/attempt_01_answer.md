The kernel uses one Chebyshev split instead of the full three-term recurrence. It computes T1, T2, T3 and T4 directly and gets the top terms from the identity T_{4+j} = 2·T4·T_j − T_{4−j}.

So y = c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3 + T4·(c4 + 2(c5T1 + c6T2 + c7T3)).

- **Ciphertext multiplications:** this needs 4 ciphertext-ciphertext multiplications instead of 6. They are z², z·T2, T2² and the final T4·q. The cost is a few more cheap scalar plaintext multiplications.
- **Depth:** depth drops from 8 to 5 levels, so the later operations also run on smaller ciphertexts.
- **Scales:** terms with different multiplication histories meet at different levels. For example, c1·T1 is at level 2, c2·T2 at level 3, c3·T3 at level 4 and T4·q at level 5. This should avoid the scale-mismatch error.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = t1 * t2;
  Ciphertext t3 = zt2 + zt2 - t1;
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;

  // q = c5 T1 + c6 T2 + c7 T3
  Ciphertext q = t1 * c5 + t2 * c6 + t3 * c7;
  Ciphertext s = q + q + c4;
  Ciphertext top = t4 * s;

  // low = (c1 - c7) T1 + (c2 - c6) T2 + (c3 - c5) T3 + c0
  Ciphertext l1 = t1 * c1 - t1 * c7;
  Ciphertext l2 = t2 * c2 - t2 * c6;
  Ciphertext l3 = t3 * c3 - t3 * c5;
  Ciphertext low = l1 + l2 + l3 + c0;

  Ciphertext y = top + low;
  y.set_output("y");
}
```