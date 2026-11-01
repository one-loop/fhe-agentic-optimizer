The baseline is a single ciphertext × plaintext-scalar multiply plus a rescale, which is already the minimum for a general scalar. The only way to change the circuit is to avoid the multiply or the rescale, and I don't see a way to do that without constants, because `s` is an unknown plaintext input.

One alternative is to build the product from additions, but that needs `s` as a ciphertext, which is impossible here. Another is to treat the scalar as a plaintext that is only encoded once, but the multiply with an encoded scalar is already the cheap path (~5 ms).

I'll resubmit the same kernel. It is the minimal circuit, so it won't register as an improvement, but I have no safe change that computes the same function with fewer operations.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```