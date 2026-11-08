I have no structurally cheaper circuit left. The remaining ideas either need plaintext-plaintext arithmetic or add plaintext operations that cost more than they save. I'm resubmitting the best kernel, which is attempt 2 and has 3 ciphertext multiplications plus 2 squares, 4 plaintext multiplies and depth 4.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t2 * c6;
  Ciphertext v = w + w + c4;
  Ciphertext m = t4 * v;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - w;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}
```