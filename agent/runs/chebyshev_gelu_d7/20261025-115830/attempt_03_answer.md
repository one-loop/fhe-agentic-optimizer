Idea: rewrite the even part in terms of w = T2 and s = w². The new form needs fewer plaintext multiplications. T4 = 2s − 1 and T6 = w·(4s − 3), so

c2·w + c4·T4 + c6·T6 = c4·(2s − 1) + w·(c2 + c6·(4s − 3)).

This uses 3 ct-ct multiplications (u, s, and w·inner) and 4 plaintext multiplications (alpha, c1, c6·s, c4·s). The c2·w multiplication disappears. The last ciphertext product lands at level 5 while the other terms are at lower levels, so there is no same-level scale clash.

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
  Ciphertext s = square(w);
  Ciphertext q = s * c6;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext inner = q4 - c6;
  inner = inner - c6;
  inner = inner - c6;
  inner = inner + c2;
  Ciphertext f = w * inner;
  Ciphertext p = s * c4;
  Ciphertext r = p + p;
  r = r - c4;
  Ciphertext result = z * c1;
  result = result + r;
  result = result + f;
  result = result + c0;
  result.set_output("y");
}
```