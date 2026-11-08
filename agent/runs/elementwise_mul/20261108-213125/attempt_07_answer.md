I'm submitting the baseline unchanged again because nothing cheaper exists. `x0 * x1` needs one ciphertext × ciphertext multiply, and CHEHAB adds the relinearize and rescale automatically. The baseline has nothing else to remove, and any different circuit only adds operations. The baseline at 6.12 ms is the final result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```