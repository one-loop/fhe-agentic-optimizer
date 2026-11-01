Resubmitting the same kernel can't win, so this attempt tests a genuinely different structure.

The ciphertext multiply now takes the fresh input x as its second operand instead of z. That moves the level alignment onto x, and alpha is applied as the last step:

y = alpha · (x · (2·c2·z + c1)) + c0 − c2

I expect it to be somewhat slower. It adds a third plaintext multiply, though a cheap one at the deepest level, and a fourth level. It does remove the alignment of z and changes the rescale pattern. If it is slower, attempt 1 stays the best.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext w = z * c2;             // level 2: c2*z
  Ciphertext inner = w + w + c1;     // 2*c2*z + c1
  Ciphertext prod = inner * x;       // level 3: x*(2c2 z + c1)
  Ciphertext scaled = prod * alpha;  // level 4: z*(2c2 z + c1)
  Ciphertext result = scaled + c0 - c2;
  result.set_output("y");
}
```