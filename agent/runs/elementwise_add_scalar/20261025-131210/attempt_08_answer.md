I have no circuit cheaper than the baseline's single add. The output depends on both inputs, so at least one operation is needed, and every distinct variant adds operations (attempt 3 measured 1.47 ms). Attempt 2 failed with a transparent ciphertext.

For the last attempt I'm submitting the baseline kernel again. The judge will mark it identical, so it cannot win, but it is the best correct kernel I have.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```