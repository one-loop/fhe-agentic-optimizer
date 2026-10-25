I have no new circuit to try. The baseline's single plaintext multiply with its rescale is the lowest cost I can reach for multiplying by a run-time scalar. The alternatives either collapse back to it under CHEHAB's rewrites or add operations and would be slower.

Here is the same minimal kernel once more.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```