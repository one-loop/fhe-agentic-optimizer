Idea: the mismatch came from `z5` and `z7` having different multiplication histories at the same level. I chain the powers so every monomial lands at its own level:
- z³ = s·z is at level 3, z⁵ = s·z³ at level 4, and z⁷ = s·z⁵ at level 5.
- That needs only 4 ct×ct multiplications, down from 6.
- After the plaintext multiplications, the terms land at levels 2, 4, 5 and 6, so CHEHAB aligns them for free.
- Depth goes up to 6, but the multiplication count drops.

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
  Ciphertext z3 = s * z;
  Ciphertext z5 = s * z3;
  Ciphertext z7 = s * z5;
  Ciphertext t3 = imul(z3, 4) - imul(z, 3);
  Ciphertext t5 = imul(z5, 16) + imul(z, 5) - imul(z3, 20);
  Ciphertext t7 = imul(z7, 64) + imul(z3, 56) - imul(z5, 112) - imul(z, 7);
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```