The baseline is a single ciphertext add, so there is no cheaper circuit. Output y needs both inputs, and combining two ciphertexts takes at least one operation. Every equivalent rewrite I could try, including `+=`, compiles to the same circuit and is rejected as identical. Any circuit that differs from the baseline has to add operations, which makes it slower.

I'm resubmitting the minimal kernel. I expect it to be reported as identical again, and I'd rather say so than present a variant that adds cost as an improvement.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```