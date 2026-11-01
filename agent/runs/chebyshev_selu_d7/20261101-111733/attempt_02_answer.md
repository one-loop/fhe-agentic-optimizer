This keeps the attempt-1 structure and changes only how T3 is built. It uses T3 = z·(2T2 − 1), written `T1 * (T2 + T2 - one)`, in place of T3 = 2·z·T2 − z.

In attempt 1, `zt + zt - T1` added T1 (level 1) to a level-3 product, so CHEHAB had to align T1. Now T1 enters only as a multiplication operand. The added `one` is a plaintext, which needs no scale alignment. This should remove one match_scale/rescale. The circuit is otherwise unchanged.

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

  Ciphertext T1 = x * alpha;
  Ciphertext zz = T1 * T1;
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext T3 = T1 * u;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext a5 = T1 * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext q = a7 + a6 + a5;
  Ciphertext inner = q + q + c4;
  Ciphertext high = T4 * inner;

  Ciphertext l1 = T1 * c1 - T1 * c7;
  Ciphertext l2 = T2 * c2 - a6;
  Ciphertext l3 = T3 * c3 - T3 * c5;
  Ciphertext low = l3 + l2 + l1;

  Ciphertext result = high + low + c0;
  result.set_output("y");
}
```