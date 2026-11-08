Idea: factor out z, so p = c0 + z·(Q(s) + constant part), where s = z².
- The constant part is z·(c1 − 3c3 + 5c5 − 7c7). I build it as four small plaintext multiplications on z, with the integer factors applied before the final add. The largest factor is only 7, which avoids the precision loss that broke attempt 6.
- The s-dependent part is c3·4s + c5·(16s² − 20s) + c7·(64s³ − 112s² + 56s). The integer combinations are built on ciphertexts before the plaintext multiplication, and the three terms land at different levels.
- One final z·W multiplication finishes the polynomial.

```cpp
Ciphertext imul(const Ciphertext &c, int n)
{
  Ciphertext acc;
  Ciphertext cur = c;
  bool has = false;
  while (n > 0)
  {
    if (n & 1)
    {
      if (has)
        acc = acc + cur;
      else
      {
        acc = cur;
        has = true;
      }
    }
    n >>= 1;
    if (n > 0)
      cur = cur + cur;
  }
  return acc;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext s2 = square(s);
  Ciphertext s3 = s2 * s;

  Ciphertext A3 = imul(s, 4);
  Ciphertext A5 = imul(s2, 16) - imul(s, 20);
  Ciphertext A7 = (imul(s3, 64) + imul(s, 56)) - imul(s2, 112);

  Ciphertext W = A3 * c3;
  W = W + A5 * c5;
  W = W + A7 * c7;

  Ciphertext zc = (z * c1 + imul(z * c5, 5)) - (imul(z * c3, 3) + imul(z * c7, 7));

  Ciphertext result = z * W;
  result = result + zc;
  result = result + c0;
  result.set_output("y");
}
```