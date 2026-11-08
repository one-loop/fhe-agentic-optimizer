This is my last attempt. I found no circuit with fewer operations than attempt 1, so I am resubmitting its minimal form (one ct-ct multiplication, two ct-pt multiplications, three levels) with a slightly different grouping. If CHEHAB produces the same circuit, the result stays at the attempt 1 kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext u = (w + w) + c1;
  Ciphertext p = z * u;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
```