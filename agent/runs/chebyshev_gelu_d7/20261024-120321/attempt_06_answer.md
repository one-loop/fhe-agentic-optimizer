I'm keeping the same kernel again. Attempt 5 measured 216.32 ms, in line with attempt 4's 217.78 ms, so the earlier 230.40 ms was noise. It still uses 3 ciphertext multiplications, 4 plaintext multiplications and a depth of 4. I found no change that lowers these counts without a plaintext-plaintext product, which CKKS rule 2 forbids.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext u = t2 * c6;
  Ciphertext v = u + u + c4;
  Ciphertext m = t4 * v;
  Ciphertext term2 = t2 * c2;
  Ciphertext term1 = z * c1;
  Ciphertext low = term2 - u;
  Ciphertext result = term1 + low;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}
```