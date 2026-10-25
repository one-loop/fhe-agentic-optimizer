I'm returning the baseline kernel again, because I have found no cheaper circuit for this task. `y = x0 * p` needs one ct-pt multiply and the rescale that follows it, and the only changes I can construct add operations. The result stays at the baseline's 12.92 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```