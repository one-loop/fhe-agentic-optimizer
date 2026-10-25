I haven't found a cheaper structure. This attempt moves the doubling to the z side of the ciphertext-ciphertext product and multiplies the c1 term separately: y = (c0 − c2) + m·(z+z) + c1·z. It should not beat the best, but it gives a different circuit to measure.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext p = m * (z + z);
  Ciphertext l = x * c1;
  Ciphertext la = l * alpha;
  Ciphertext y = ((p + la) + c0) - c2;
  y.set_output("y");
}
```