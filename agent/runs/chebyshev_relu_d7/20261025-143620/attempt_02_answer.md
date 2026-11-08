The first attempt failed because `c4·T4` and the `(c6·T2)·T4` term met at level 4 with different multiplication histories. I fold c4 into the product: `(2·c6·T2 + c4)·T4` is a single multiplication, so those two terms become one. The remaining terms (`c2·T2 - c6·T2` and `c1·z`) sit at levels 3 and 2, so CHEHAB aligns them with the level-4 term for free.

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
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a + c4;
  Ciphertext p = a2 * t4;
  Ciphertext u = t2 * c2;
  Ciphertext w = z * c1;
  Ciphertext lower = u - a;
  Ciphertext result = p + lower;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```