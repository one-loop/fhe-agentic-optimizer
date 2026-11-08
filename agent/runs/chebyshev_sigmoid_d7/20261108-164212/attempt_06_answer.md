Rather than multiplying z·c1 − z·c5 − z·c7 separately, I fold the constant c1 − c5 − c7 into the factor that multiplies z, using plaintext additions only. The result is y = z·(u·inner + c1 − c5 − c7) + c0, with u = 2·T2 − 1 and inner as before.

- This needs 3 ciphertext-ciphertext multiplications (w, u·inner, and the final z multiplication) and only 3 plaintext multiplications (alpha, c7·T2, and one more).
- It costs one extra level, 6 instead of 5, so it may or may not be a net win. I have not compiled or timed it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext m = t2 * c7;
  m = m + m;
  m = m + m;
  m = m + c5;
  m = m + c5;
  Ciphertext inner = t2 * m;
  inner = inner + c3;
  inner = inner - c7;
  inner = inner - c7;
  Ciphertext q = u * inner;
  q = q + c1;
  q = q - c5;
  q = q - c7;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
```