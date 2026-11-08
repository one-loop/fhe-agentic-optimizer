I don't see another change that lowers the cost further. The two `*one` multiplies already use the full 10 levels, so burning more is not possible. Every variant I considered that removes a ct-ct multiply adds more plaintext multiplies than it saves, and at depth 10 the burn-level trick leaves no room for them. I'm resubmitting the best kernel (186.56 ms, about 1.9× faster than the 358.66 ms baseline).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext x1 = x * one;
  Ciphertext x2 = x1 * one;
  Ciphertext z = x2 * alpha;
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