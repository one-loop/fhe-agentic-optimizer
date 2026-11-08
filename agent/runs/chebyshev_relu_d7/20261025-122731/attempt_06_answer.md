Idea: start from the best kernel (attempt 2) and drop the plaintext subtraction that builds T4. Since T4 = 2·s − 1 with s = t2², I get T4·v = s·(2v) − v. That is the same number of multiplies and the same depth, but it saves a plaintext subtraction (about 2.5 ms of encoding) and the extra term. The `-v` merges into the final sum as a cheap ciphertext subtraction.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext q = t2 * c6;
  Ciphertext r = q + q;
  Ciphertext v = r + c4;
  Ciphertext vv = v + v;
  Ciphertext w = s * vv;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - q;
  result = result + w;
  result = result - v;
  result = result + c0;
  result.set_output("y");
}
```