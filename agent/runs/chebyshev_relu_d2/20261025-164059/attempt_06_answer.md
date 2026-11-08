I'm trying a small structural variation of attempt 1. It doubles z before the plaintext multiplication instead of after. The operation counts should be the same, but the scale and level handling may differ.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext v = zz * c2;
  Ciphertext w = v + c1;
  Ciphertext r = w * z;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```