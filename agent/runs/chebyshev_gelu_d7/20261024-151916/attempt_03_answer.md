Idea: shorten the critical path from depth 5 to 4. I multiply `w` by `c6` early, in parallel with `square(w)`, and then multiply that product by `s = 4w² − 3`. The final result is

c6·w·s + 2c4·w² + c2·w + c1·z + (c0 − c4).

This costs one extra plaintext multiplication but removes a level.

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
  Ciphertext d = w2 * c4;
  Ciphertext result = b + d + d;
  result = result + w * c2;
  result = result + z * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```