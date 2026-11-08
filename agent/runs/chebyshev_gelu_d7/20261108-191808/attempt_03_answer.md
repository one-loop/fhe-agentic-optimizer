The 4-level structure looks like a floor, since the chain alpha → z² → u² → T4·w is unavoidable. So this attempt cuts plaintext encodings, which cost about 2.5 ms each.

T4 = 2u² − 1, so T4·w = 2(u²·w) − w. Subtracting w is a ciphertext-ciphertext operation, so the separate `t4 = uu + uu - one` step, which needs a plaintext add, is no longer needed. That removes one plaintext add and two ciphertext adds, at the cost of one ciphertext subtraction. The rest of the circuit is unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext zz = z * z;               // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext uu = u * u;               // level 3
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext p = uu * w;               // level 4
  Ciphertext mid = u * c2 - c6u - w;   // level 3
  Ciphertext lin = z * c1 + c0;        // level 2
  Ciphertext result = p + p + (mid + lin);
  result.set_output("y");
}
```