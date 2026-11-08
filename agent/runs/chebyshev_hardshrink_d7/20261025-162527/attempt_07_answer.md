The `square(z)` kernel (attempt 6) ran at 268.04 ms in its single timing, but it was not confirmed faster than attempt 2. This variant keeps `square` and also uses `square(t2)` to build T4. It then forms T5 as 2·T2·T3 − T1 and T7 as 2·T3·T4 − T1 so the two high terms sit at the same depth. That costs one more squaring but shortens the T7 chain, and both high terms are built from the same kind of products. I'm adding it as a different-circuit probe.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t2 * t5;
  Ciphertext t7 = p7 + p7 - t3;
  Ciphertext a = t7 * c7;
  Ciphertext b = t5 * c5;
  Ciphertext c = t3 * c3;
  Ciphertext d = z * c1;
  Ciphertext result = a + (b + (c + d));
  result.set_output("y");
}
```