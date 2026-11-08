The baseline needs depth 3: the term 2·α²·c2·x² has five factors, and a binary tree with five leaves has depth at least 3. So the saving has to come from fewer operations. I rewrite the polynomial in Horner form:

y = z·(2·c2·z + c1) + c0 − c2, with z = α·x.

This uses 2 ciphertext×plaintext multiplies instead of 3, plus 1 ciphertext×ciphertext multiply. Writing 2·c2·z as `w + w` follows the no-constants rule.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext v = w + w + c1;
  Ciphertext p = z * v;
  Ciphertext y = p - c2 + c0;
  y.set_output("y");
}
```