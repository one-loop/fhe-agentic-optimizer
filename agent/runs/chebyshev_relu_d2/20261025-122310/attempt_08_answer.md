I couldn't find a circuit that beats attempt 2, so this final attempt resubmits it unchanged: it computes y = (c1 + 2·c2·z)·z + c0 − c2 with z = x·alpha.

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