Two rewrites gave identical circuits to attempt 1, so this one changes the structure: z·c1 and 2c2·z² are accumulated separately instead of folding c1 into the factor. It spends an extra ct-pt multiplication, so it will probably lose. It still shows whether the single deepest ct-ct multiplication being more balanced shifts the cost.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext r = z * (w + w);
  Ciphertext t = z * c1;
  Ciphertext y = ((r + t) + c0) - c2;
  y.set_output("y");
}
```