Idea: the last failure came from `t3*c3` and `t4*c4` meeting at the same level with different multiplication histories. `t3 = z*t2` multiplies a depth-1 value by a depth-2 value, while `t4 = t2*t2` multiplies two depth-2 values. I now give every multiplicand the same history by lifting `z` and `t2` with cheap multiplications by `one`: `z1 = z*one` and `t2b = t2*one`. Then t3, t4 and the degree 5–7 products all have matching scale patterns.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z1 = z * one;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext t2b = t2 * one;
  Ciphertext zt2 = z1 * t2;
  Ciphertext t3 = zt2 + zt2 - z1;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext t3t2 = t3 * t2b;
  Ciphertext t5 = t3t2 + t3t2 - z1;
  Ciphertext t3s = t3 * t3;
  Ciphertext t6 = t3s + t3s - one;
  Ciphertext t4t3 = t4 * t3;
  Ciphertext t7 = t4t3 + t4t3 - z1;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext r = t3 * c3;
  r = r + t4 * c4;
  r = r + t5 * c5;
  r = r + t6 * c6;
  r = r + t7 * c7;
  Ciphertext low = z * c1;
  low = low + t2 * c2;
  r = r + low;
  r = r + c0;
  r.set_output("y");
}
```