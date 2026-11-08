Idea: apply attempt 5's ordering (the shallow term last) to the Horner form that avoids t4. This keeps the 3 ciphertext multiplications and 4 plaintext multiplications, drops one subtraction and one doubling, and adds the shallow term last.

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
  Ciphertext a2 = a + a;
  Ciphertext q = a2 + c4;
  Ciphertext r = s2 * q;
  Ciphertext low = t2 * c2 - a2 - a;
  Ciphertext mid = r + r + low;
  mid = mid - c4;
  mid = mid + c0;
  Ciphertext result = mid + z * c1;
  result.set_output("y");
}
```