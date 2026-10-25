This attempt, `b + x * a`, gave 18.34 ms, which is within noise of the others. All five runs compile to the same graph (1 ct-pt multiply, 1 rescale, 1 add) and land between 18.14 and 18.42 ms.

I see no remaining way to cut work. Both plaintexts must be encoded and used once, and the rules forbid constants and plaintext-plaintext arithmetic, so `b` can't be pre-merged with `a`. Any rewrite either adds operations or compiles to the same thing. I'm resubmitting the best-measured form, the baseline expression, and I don't expect a real gain from further attempts.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```