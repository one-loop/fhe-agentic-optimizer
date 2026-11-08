I have no new idea that could be cheaper, so I'm not using the last attempts on circuits already evaluated. One ciphertext multiplication (with its relinearize and rescale) is the minimum for x². Without plaintext inputs there are no constants, and I can't emit mod switches, so every other formulation I can build is slower or computes the wrong function. The baseline `x * x` therefore stays as the best kernel, repeated below.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```