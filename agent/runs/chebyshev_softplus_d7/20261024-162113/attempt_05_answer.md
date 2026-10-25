Idea: keep attempt 4's structure, but save a plaintext subtraction. Since T4 = 2s − 1 with s = T2², T4·b = (s+s)·b − b. That removes the `− one` used to build T4 and replaces it with a cheap ciphertext subtraction.

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
  Ciphertext a = t2 * c6;
  Ciphertext b = a + a + c4;
  Ciphertext main = (s + s) * b;
  Ciphertext low = t2 * c2 - a - b;
  Ciphertext result = main + low;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
```