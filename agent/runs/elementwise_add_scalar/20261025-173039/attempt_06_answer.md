I have no cheaper circuit to offer. The kernel is one ciphertext-plaintext add, which is the minimum for computing x0 + s while using both inputs. Every alternative I can build either normalizes to the same circuit, as my earlier resubmissions did, or adds operations such as negations, multiplications or alignments. Each of those costs more than the 0.5 ms add.

I'm returning the same kernel again. It is correct, but it cannot be confirmed as a new best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```