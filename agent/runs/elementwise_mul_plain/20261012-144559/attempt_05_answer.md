I'm resubmitting the same kernel. Every attempt so far has produced the same single ciphertext-plaintext multiply, at 7.03 to 7.05 ms. That spread is noise, and no alternative formulation can remove the one required multiply, so there is nothing further to gain.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```