Because alpha = 0.25, 4z² = x²/4, so `s = square(x) * alpha` gives s directly. The square then runs on the fresh input at the top level, where it is cheaper. `z = x * alpha` is computed in parallel and used only in the final multiply. The op counts match the best kernel, so any gain should come from the depth and level layout. I haven't timed it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext x2 = square(x);
  Ciphertext s = x2 * alpha;
  Ciphertext s3 = s - one;
  s3 = s3 - one;
  s3 = s3 - one;
  Ciphertext s5 = s3 - one;
  s5 = s5 - one;
  Ciphertext r = s * s5;
  Ciphertext r5 = r + one;
  r5 = r5 + one;
  r5 = r5 + one;
  r5 = r5 + one;
  r5 = r5 + one;
  Ciphertext g = r5 * c5 + s3 * c3;
  g = g + c1;
  Ciphertext z = x * alpha;
  Ciphertext y = z * g;
  y = y + c0;
  y.set_output("y");
}
```