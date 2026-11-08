The circuit has to differ from every earlier attempt, and I still have no cheaper structure. I'm trying a distinct negation arrangement, which gives a different operation count. It will likely be no faster than the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext m = nx * a;
  Ciphertext k = m - b;
  Ciphertext y = -k;
  y.set_output("y");
}
```