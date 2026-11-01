This is another run of the same kernel, at 7.04 ms against a 7.03 ms baseline. The difference is within run-to-run noise. The kernel performs one ciphertext-plaintext multiply and nothing else, which is the minimum the task allows, so I see no further improvement to try. I'm keeping the baseline as the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```