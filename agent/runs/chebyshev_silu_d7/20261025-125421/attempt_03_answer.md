Idea: avoid building t4 explicitly. Using t4 = 2s − 1, with s = t2², we get t4·v = 2·s·v − v. This drops the plaintext subtraction of `one` that built t4 and replaces it with a cheaper ciphertext subtraction. The multiplication count and depth stay the same.

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
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext w = t2 * c6;
  Ciphertext v = w + w + c4;
  Ciphertext sv = s * v;
  Ciphertext m = sv + sv - v;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - w;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}
```