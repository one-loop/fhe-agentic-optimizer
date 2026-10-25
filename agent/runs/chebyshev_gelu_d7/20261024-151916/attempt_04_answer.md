Idea: attempt 3 failed because the two level-4 terms had different multiplication histories. `w2 * c4` is ct-ct then plaintext, while `(w*c6) * s` is plaintext then ct-ct. I rebuild the c4 term as `(w*c4) * (w+w)`, which has the same plaintext-then-ct-ct pattern, so the sum is valid at level 4. This costs one extra ciphertext multiplication but keeps depth at 4.

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
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext s = t4 + t4 - one;
  Ciphertext a = w * c6;
  Ciphertext b = a * s;
  Ciphertext e = w * c4;
  Ciphertext d = e * (w + w);
  Ciphertext result = b + d;
  result = result + w * c2;
  result = result + z * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```