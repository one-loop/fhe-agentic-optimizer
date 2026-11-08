I have no structurally different circuit that beats attempt 2, so this is the same best kernel again.

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