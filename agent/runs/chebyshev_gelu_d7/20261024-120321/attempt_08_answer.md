I'm submitting the same kernel one last time. Attempt 7 measured 215.97 ms against the 314.70 ms baseline, about 31% faster. It uses 3 ciphertext multiplications, 4 plaintext multiplications and a depth of 4, down from 5 and 5 multiplications and a depth of 7 in the baseline. I have no remaining change that lowers these counts, so I'm not risking the last attempt on an untested variation.

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