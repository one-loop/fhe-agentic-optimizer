I'm stopping here rather than submitting another identical kernel. The timing is 4.59–4.60 ms across runs, which is noise, and the kernel has one ciphertext-plaintext multiply and one rescale. Nothing in the DSL can remove either without changing the computation.

I'm not spending the remaining attempts on guesses. The baseline is the best kernel found:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```