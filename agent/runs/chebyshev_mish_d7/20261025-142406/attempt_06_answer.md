I don't have a change that I expect to beat the attempt 3 kernel (251.79 ms, about 1.42× faster than the baseline). The two shallower alternatives I tried were slower: the T4 split (attempt 4, 331 ms) and Clenshaw in T2 (attempt 5, 307 ms). The other variants I considered do not reduce ct-ct multiplies, and the ones that remove them add more plaintext multiplies than they save. I'm resubmitting the best kernel unchanged, which cannot win since its circuit matches an earlier attempt.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext b6 = z2 * c7 + c6;
  Ciphertext b5 = z2 * b6 + c5 - c7;
  Ciphertext b4 = z2 * b5 + c4 - b6;
  Ciphertext b3 = z2 * b4 + c3 - b5;
  Ciphertext b2 = z2 * b3 + c2 - b4;
  Ciphertext b1 = z2 * b2 + c1 - b3;
  Ciphertext y = z * b1 + c0 - b2;
  y.set_output("y");
}
```