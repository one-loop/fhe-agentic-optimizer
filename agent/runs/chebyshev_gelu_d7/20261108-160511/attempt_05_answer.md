This variant is the attempt 4 kernel with two small changes. The c2 term is computed from u as 2·c2·u − c2 instead of from w, and the c4 constant is folded into one subtraction. The multiplication structure is unchanged, so the gain, if any, should be small. I have not compiled or run it.

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
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;          // T2
  Ciphertext s = w * w;
  Ciphertext r = w * c6;
  Ciphertext r2 = r + r;
  Ciphertext r4 = r2 + r2;
  Ciphertext v = r4 + c4 + c4;
  Ciphertext t = s * v;
  Ciphertext r3 = r2 + r;
  Ciphertext uc = u * c2;
  Ciphertext t2 = uc + uc;             // 2*c2*u
  Ciphertext t1 = z * c1;
  Ciphertext lin = t2 + t1;
  Ciphertext result = t - r3;
  result = result + lin;
  result = result - c4;
  result = result - c2;
  result = result + c0;
  result.set_output("y");
}
```