The circuit is already a single ciphertext-plaintext add, which is the minimum for using both inputs. I have no valid way to make it cheaper. Every alternative either normalizes to the same circuit or adds operations. I'm submitting it one last time. It is correct but cannot be confirmed as a new best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```