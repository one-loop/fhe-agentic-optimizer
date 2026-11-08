The last Horner version tied attempt 3, so the remaining gain is in the roughly 30 ciphertext additions. I factor the integer weights as `Q3 = 64d`, `Q2 = 16·(c − 7d)`, `Q1 = 4·(b − 5c + 14d)` and share common sub-sums. I also reorder the Horner steps to cut the add count. The ct-ct and plaintext multiplication counts stay the same.

```cpp
Ciphertext dbl(const Ciphertext &c, int k)
{
  Ciphertext r = c;
  for (int i = 0; i < k; ++i)
    r = r + r;
  return r;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext a = z * c1;
  Ciphertext b = z * c3;
  Ciphertext c = z * c5;
  Ciphertext d = z * c7;
  Ciphertext d2 = d + d;
  Ciphertext d8 = dbl(d2, 2);
  Ciphertext c4 = dbl(c, 2);
  Ciphertext e = c - (d8 - d);              // c - 7d
  Ciphertext f = (b - (c4 + c)) + dbl(d, 4) - d2; // b - 5c + 14d
  Ciphertext g = (c4 + c) - (d8 - d);       // 5c - 7d
  Ciphertext S = (a - (b + b + b)) + g;
  Ciphertext Q3 = dbl(d, 6);
  Ciphertext Q2 = dbl(e, 4);
  Ciphertext Q1 = dbl(f, 2);
  Ciphertext h = Q2 + w * Q3;
  h = Q1 + w * h;
  Ciphertext r = S + w * h;
  r.set_output("y");
}
```

I haven't checked this numerically. `f = b − 5c + 14d` is built as `b − (4c + c) + 16d − 2d`.