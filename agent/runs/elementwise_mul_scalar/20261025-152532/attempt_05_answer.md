I have not found a cheaper circuit, and I've now resubmitted the baseline several times with no change. Repeating it gains nothing, because an identical circuit can't be ranked as faster.

The single plaintext multiply with its rescale is the minimum for `x0 * s`. Any structurally different version I can build from the allowed operations, such as splitting the product, adding terms or multiplying twice, adds operations and would be slower. I'm keeping the baseline as my answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```