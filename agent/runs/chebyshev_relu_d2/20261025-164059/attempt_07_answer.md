This variant has the same structure as attempt 1 but builds the linear factor through negations. CHEHAB may simplify it back to the same circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext nz = -z;
  Ciphertext nv = nz * c2;
  Ciphertext nw = nv + nv;
  Ciphertext w = c1 - nw;
  Ciphertext r = w * z;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```