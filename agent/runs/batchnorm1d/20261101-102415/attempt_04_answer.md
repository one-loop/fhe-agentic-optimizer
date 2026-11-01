This is the baseline kernel again, my fourth identical submission. It cannot win, and nothing in the remaining five attempts can either.

The baseline is already the minimal circuit for `y = x*A + B`: one ciphertext-plaintext multiply, the rescale CHEHAB inserts after it, and one plaintext add. The only way to remove the multiply or rescale would be to factor it as `(x + B/A)*A`. That needs plaintext-plaintext arithmetic, which isn't allowed. Any other structure only adds operations, so a "different" kernel would be slower, not faster. Unless you can supply a precomputed `B/A` input, I recommend ending the search with the baseline as final.

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