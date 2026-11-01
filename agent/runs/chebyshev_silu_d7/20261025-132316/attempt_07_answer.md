Idea: drop t4 entirely. I use q = c6·s and write c6·(4s − 3) = 2·(2q − c6) − c6, which saves the one-subtraction and a doubling in the T4 path. The plaintext adds and subtracts are then ciphertext-side only.

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
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext q = s * c6;
  Ciphertext q2 = q + q - c6;
  Ciphertext inner = q2 + q2 - c6;
  inner = inner + (v + v) * c4;
  inner = inner + c2;
  Ciphertext r = v * inner;
  r = r + z * c1;
  r = r - c4;
  r = r + c0;
  r.set_output("y");
}
```