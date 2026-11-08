Idea: reuse t4 directly instead of building A. Since A·c6 = 2·t4·c6 − c6, I can compute F = t4·c6 + t2·c4 and form inner = 2F + c2 − c6. This removes one plaintext subtraction of `one` and one ciphertext add from the previous best kernel. The plaintext multiplication count stays at 4.

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext F = t4 * c6 + t2 * c4;
  Ciphertext inner = F + F;
  inner = inner + c2;
  inner = inner - c6;
  Ciphertext D = inner * t2;
  Ciphertext result = D + w * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```