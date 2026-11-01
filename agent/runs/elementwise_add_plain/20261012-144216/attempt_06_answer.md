I'm stopping here rather than spending more attempts. The kernel is one ciphertext-plaintext add, which is the minimum the task allows. Across runs the time stays between 2.51 and 2.71 ms with the same operation count, so the variation is measurement noise, not something the kernel controls. The fastest result observed was 2.51 ms in attempt 3, and that kernel is identical to this one:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```