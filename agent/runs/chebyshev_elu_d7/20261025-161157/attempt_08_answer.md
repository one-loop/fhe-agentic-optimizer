Idea: same as the best kernel (attempt 3), but add `c0` to the final result at the deepest level instead of into A. Encoding the plaintext at a lower level, with fewer remaining moduli, should be slightly cheaper, and the add order changes the circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;

  Ciphertext zc5 = z * c5;
  Ciphertext t2c6 = t2 * c6;
  Ciphertext t3c7 = t3 * c7;
  Ciphertext b = (zc5 + zc5);
  b = b + (t2c6 + t2c6);
  b = b + (t3c7 + t3c7);
  b = b + c4;
  Ciphertext hi = t4 * b;

  Ciphertext a = z * c1 - z * c7;
  a = a + (t2 * c2 - t2 * c6);
  a = a + (t3 * c3 - t3 * c5);

  Ciphertext r = a + hi;
  r = r + c0;
  r.set_output("y");
}
```