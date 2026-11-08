I haven't found a circuit with fewer operations than attempt 1. It is already at 1 ciphertext-ciphertext multiplication and 2 plaintext multiplications. This is my final submission, and it is attempt 1's kernel with a cleaner structure. It is probably the same circuit, so it should match the best time.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = z * c2;
  Ciphertext w = (v + v) + c1;
  Ciphertext r = w * z;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```