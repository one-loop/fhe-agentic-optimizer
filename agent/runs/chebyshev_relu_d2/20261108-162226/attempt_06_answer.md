This last variant computes the c2 scaling from x directly and then applies alpha, so it feeds the same Horner product in a different order. The operation counts should match attempt 1, and it may be reported as the same circuit. I have not compiled or run it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext a = x * c2;
  Ciphertext a2 = a + a;
  Ciphertext v = a2 * alpha + c1;
  Ciphertext z = x * alpha;
  Ciphertext w = z * v;
  Ciphertext result = w - c2;
  result = result + c0;
  result.set_output("y");
}
```