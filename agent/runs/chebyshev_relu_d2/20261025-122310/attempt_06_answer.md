I haven't found a circuit cheaper than attempt 2. Its form, (c1 + 2·c2·z)·z + c0 − c2 with z = x·alpha, is the best I've found: 2 plaintext multiplications, 1 ciphertext multiplication, 3 rescales and 1 mod switch. This attempt resubmits it unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext g = z * c2;
  Ciphertext g2 = g + g;
  Ciphertext m = g2 + c1;
  Ciphertext r = m * z;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```