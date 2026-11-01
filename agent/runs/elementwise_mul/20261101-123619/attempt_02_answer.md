That result is what I predicted: the circuit was identical to the baseline's, so it was not timed. No cheaper circuit exists for this function, so I'll stop here instead of using the remaining attempts on variants that can only be slower.

The baseline uses one ciphertext multiplication with its required relinearization and rescale, at one level, and nothing else. A product of two independent encrypted inputs needs at least one ciphertext-ciphertext multiplication. There are no rotations, plaintext operations, additions or alignments to remove, and one level is the minimum depth for any product.

Every different circuit computing `x0 * x1` would add work: more multiplications, rotations, plaintext operations or additions. None could beat the baseline by 3%. The baseline remains the best kernel:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```