Idea: this is my last attempt, so I take attempt 2, the best kernel so far, and change only how `s = t2²` is computed. I use `square(t2)` while keeping `u = z·z` as an ordinary multiply. Attempt 4 used `square` in both places and was slower, so this tests whether a single `square` on the deeper, cheaper level helps. The other terms are unchanged.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext q = t2 * c6;
  Ciphertext r = q + q;
  Ciphertext v = r + c4;
  Ciphertext w = t4 * v;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - q;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```