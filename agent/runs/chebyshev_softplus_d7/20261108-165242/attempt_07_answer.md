This variant removes the t4 subtraction by folding it into the polynomial. With d = 2·c6·s + c4, we have T4·d = q·(2d) − d, so y = q·(2d) + c2·s − 3·c6·s − c4 + c1·z + c0. The circuit stays at 3 ciphertext multiplications and 4 levels, but the add and subtract structure differs.

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
  Ciphertext u = square(z);
  Ciphertext s = u + u - one;
  Ciphertext q = square(s);
  Ciphertext c6s = s * c6;
  Ciphertext e = c6s + c6s;
  Ciphertext d = e + c4;
  Ciphertext f = d + d;
  Ciphertext c2s = s * c2;
  Ciphertext lin = z * c1;
  Ciphertext main_term = q * f;
  Ciphertext low = c2s - e - c6s;
  Ciphertext result = main_term + low + lin;
  result = result - c4 + c0;
  result.set_output("y");
}
```