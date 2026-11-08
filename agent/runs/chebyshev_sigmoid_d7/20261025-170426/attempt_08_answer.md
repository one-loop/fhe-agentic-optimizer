Idea: this keeps the attempt-4 chain z, s, z³, z⁵, z⁷ (4 ct×ct multiplications) and pulls all the z-linear parts out. The z coefficient c1 − 3c3 + 5c5 − 7c7 is built from four small plaintext multiplications on z, with integer factors up to 7 only. The remaining Chebyshev parts then use only z³, z⁵ and z⁷ with integer factors, which cuts both the additions and the cross-level alignments on the z terms.

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

  Ciphertext zc = (z * c1 + imul(z * c5, 5)) - (imul(z * c3, 3) + imul(z * c7, 7));

  Ciphertext t3 = imul(z3, 4);
  Ciphertext t5 = imul(z5, 16) - imul(z3, 20);
  Ciphertext t7 = (imul(z7, 64) + imul(z3, 56)) - imul(z5, 112);

  Ciphertext result = zc;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```