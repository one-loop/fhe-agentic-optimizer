I don't think any structurally different kernel can beat the baseline here.

`y = x²` already costs one ciphertext multiplication, one relinearization and one rescale at one level. Nothing else in the circuit can be removed:

- **Extra operations** (adds, rotations, plaintext multiplies) only add work. Squaring `x` cannot be split into cheaper pieces.
- **Polynomial rewrites** such as `((x+x)*x)` with a correction term need extra multiplications or constants that CKKS cannot express. They would also consume another level, which makes every operation more expensive.
- **Relinearization and rescale** are inserted by CHEHAB, and the DSL gives no way to skip them.

So every new circuit I could submit would be correct but slower, and could not be confirmed faster. The only variant that differs from the baseline, `square(x)`, was about 2% faster, under the 3% threshold. I suggest ending this task with the baseline as the best result. Below is the best circuit found, for reference; it is the same as attempt 1.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```