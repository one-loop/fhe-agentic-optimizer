Idea: change where c7 enters. I form Z7 = z·c7 early, in parallel with the other work at a shallow level, and multiply it directly by the cubic A7. The rest, z·(c3·A3 + c5·A5 + K), is a separate product. This adds one ciphertext multiplication but cuts the multiplicative depth from 6 to 5, which removes the deepest plaintext multiplication and some alignments.

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
  Ciphertext z = x * alpha;
  Ciphertext w = square(z);
  Ciphertext Z7 = z * c7;
  Ciphertext u = w - one;
  Ciphertext p = w * u;
  auto dbl = [](const Ciphertext &a) { return a + a; };
  Ciphertext w64 = dbl(dbl(dbl(dbl(dbl(dbl(w))))));
  Ciphertext wp64 = w64 * p;
  Ciphertext u2 = dbl(u);
  Ciphertext u4 = dbl(u2);
  Ciphertext u8 = dbl(u4);
  Ciphertext u3 = u2 + u;
  Ciphertext A3 = w + u3;
  Ciphertext p16 = dbl(dbl(dbl(dbl(p))));
  Ciphertext p48 = p16 + dbl(p16);
  Ciphertext A5 = p16 - u4;
  Ciphertext A7 = (wp64 - p48) + u8;
  Ciphertext S = A3 * c3;
  S = S + c1;
  S = S + c5;
  S = S + c7;
  S = S + A5 * c5;
  Ciphertext y = z * S + Z7 * A7;
  y.set_output("y");
}
```