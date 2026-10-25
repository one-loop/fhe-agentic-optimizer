Same structure as attempt 5, with one fewer plaintext subtract (about 2.5 ms of encoding). Computing 4w² − 3 as t4 + t4 − one, with t4 = 2w² − one, needs two plaintext subtracts instead of three.

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
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext sp = t4 + t4 - one;
  Ciphertext a = w * c4;
  Ciphertext inner = sp * c6;
  inner = inner + a + a;
  inner = inner + c2;
  Ciphertext result = w * inner;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```