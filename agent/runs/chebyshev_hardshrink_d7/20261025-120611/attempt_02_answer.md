The first attempt failed because terms added at the same level had different scales. I now add `c5*t5` and `c7*t7` together first, since they are both products of the same shape at the same depth. The lower-degree terms `c3*t3` and `c1*z` sit at lower levels, so CHEHAB aligns them for free. The circuit is otherwise the same as before: a depth-5 chain with 5 ct-ct multiplications.

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
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = t2 * t2;
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a5 = t3 * t2;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext a7 = t4 * t3;
  Ciphertext t7 = a7 + a7 - z;
  Ciphertext hi = t5 * c5 + t7 * c7;
  Ciphertext lo = t3 * c3 + z * c1;
  Ciphertext r = hi + lo;
  r.set_output("y");
}
```