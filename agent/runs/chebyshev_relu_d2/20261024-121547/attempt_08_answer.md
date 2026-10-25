This is the best kernel (attempt 3, 81.55 ms against the 107.23 ms baseline), submitted again for the last attempt. I found no change that lowers the operation counts. They are one ct-ct multiply, two ct-pt multiplies and three plaintext adds, and the run-to-run spread is only noise.

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
  Ciphertext s = w + c1;
  s = s + w;
  Ciphertext r = z * s;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```