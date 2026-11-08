This variant moves the doubling to after the ciphertext multiplication, so the add that doubles u moves to the lower-level product. The operation counts should be about the same, so I expect little or no speedup. I have not compiled or run it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext u = z * c2;
  Ciphertext v = u + c1;
  Ciphertext w = z * v;
  Ciphertext result = w + w;
  result = result + c0;
  result = result - c2;
  result.set_output("y");
}
```

Wait — this changes the math: doubling after the multiply gives 2·z·(c2·z + c1) = 2c2·z² + 2c1·z, so the linear term is doubled. That is wrong, so it will fail correctness. Here is the corrected kernel, which keeps the best structure and drops the incorrect doubling:

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
  Ciphertext u = zz * c2;
  Ciphertext v = u + c1;
  Ciphertext w = z * v;
  Ciphertext result = w + c0;
  result = result - c2;
  result.set_output("y");
}
```