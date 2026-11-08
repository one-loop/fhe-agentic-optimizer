This attempt tries a different form: negate the plaintext B directly and subtract it. That avoids the two extra ciphertext negations of the last attempt, which added adds and depth. The multiply and rescale are unchanged. If CHEHAB rejects negating a plaintext, I will return to the baseline form.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t - (-b);
  y.set_output("y");
}
```