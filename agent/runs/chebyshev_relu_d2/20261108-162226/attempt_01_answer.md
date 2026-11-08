The idea is to factor the polynomial with Horner's rule: y = z·(c1 + 2·c2·z) + c0 − c2, where z = alpha·x. Since 2·c2·z·z = 2·c2·z² and T2 = 2z² − 1, this equals c0 + c1·z + c2·T2. This removes one plaintext multiplication, because the baseline needs three and this version needs two. It also uses one fewer ciphertext add. The plaintext `one` is no longer needed. The depth stays at 3 and the ciphertext-ciphertext multiplication count stays at 1. I have not compiled or run this.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext u = z * c2;
  Ciphertext v = u + u + c1;
  Ciphertext w = z * v;
  Ciphertext result = w + c0;
  result = result - c2;
  result.set_output("y");
}
```