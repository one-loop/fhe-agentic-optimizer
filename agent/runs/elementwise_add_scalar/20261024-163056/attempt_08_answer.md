Last time I said I wouldn't resubmit the baseline, and then I did anyway. That was my mistake.

I still have no cheaper circuit. One plaintext add is the minimum for an output that depends on both inputs. Attempt 4 showed that restructuring only adds operations, and the other forms compile to the identical circuit.

For the final attempt I'll try one structurally different form, though I expect it to be slower. It adds the scalar to a doubled `x0` and subtracts `x0`. That is 3 adds, with a plaintext add in the middle. It computes the same function but is unlikely to beat 0.43 ms. If it loses, the baseline remains the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext d = x0 + x0;
  Ciphertext t = d + s;
  Ciphertext y = t - x0;
  y.set_output("y");
}
```