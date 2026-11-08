Idea: skip building T4 as 2·q − 1. Since T4·B = 2·q·B − B with q = T2², I multiply q·B directly, double it, and subtract B. This saves one plaintext add (encoding) and one op on T4. I haven't compiled or run this kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext p = z * t2;
  Ciphertext t3 = p + p - z;
  Ciphertext q = t2 * t2;

  Ciphertext u5 = z * c5;
  Ciphertext u6 = t2 * c6;
  Ciphertext u7 = t3 * c7;
  Ciphertext B = u5 + u5 + u6 + u6 + u7 + u7 + c4;

  Ciphertext prod = q * B;

  Ciphertext a1 = z * c1;
  Ciphertext a7 = z * c7;
  Ciphertext a2 = t2 * c2;
  Ciphertext a3 = t3 * c3;
  Ciphertext a5 = t3 * c5;
  Ciphertext A = a1 - a7;
  A = A + a2;
  A = A - u6;
  A = A + a3;
  A = A - a5;
  A = A + c0;
  A = A - B;

  Ciphertext y = A + prod + prod;
  y.set_output("y");
}
```