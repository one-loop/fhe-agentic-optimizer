Idea: expand t4·q = 2·s2·q − q, which removes the t4 subtraction and its plaintext encoding. This is attempt 5's structure otherwise.

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext q = a + a + c4;
  Ciphertext r = s2 * q;
  Ciphertext low = t2 * c2 - a;
  low = low - q;
  Ciphertext mid = r + r + low;
  mid = mid + c0;
  Ciphertext result = mid + z * c1;
  result.set_output("y");
}
```