Last attempt. Idea: a variant of attempt 2 that skips building w. I use c6·w = 2·(c6·T4) − c6, so the inner factor is built from c6·T4 with a ciphertext add and two plaintext add/subtracts. This removes one plaintext subtraction, the `− one` that built w.

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
  Ciphertext t4 = s + s - one;
  Ciphertext tc = t4 * c6;
  Ciphertext inner = tc + tc - c6 + c2;
  Ciphertext result = z * c1;
  result = result + t4 * c4;
  result = result + t2 * inner;
  result = result + c0;
  result.set_output("y");
}
```