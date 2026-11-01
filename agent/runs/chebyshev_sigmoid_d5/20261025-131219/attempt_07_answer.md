This variant keeps 3 ciphertext multiplications but evaluates the polynomial as zw = z·w and zw2 = zw·w. It applies c3 and c5 to the combined terms, and builds the linear part from the shared sums 5z and 3z.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext w = u2 + u2;
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw * w;
  Ciphertext z2 = z + z;
  Ciphertext z3 = z2 + z;
  Ciphertext z5 = (z2 + z2) + z;
  Ciphertext zwx2 = zw + zw;
  Ciphertext zw5 = (zwx2 + zwx2) + zw;
  Ciphertext D = zw2 - zw5;
  Ciphertext lin = (z * c1 + z5 * c5) - z3 * c3;
  Ciphertext y = (zw * c3 + D * c5) + lin;
  y = y + c0;
  y.set_output("y");
}
```