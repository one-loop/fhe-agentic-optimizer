I have no further change that I expect to beat the current best, so the best kernel stays as attempt 2 (about 56 ms vs 169 ms for the baseline).

The gains came from evaluating the even part as a polynomial in w = T2 = 2z² − 1. That cut depth from 7 to 4 and ciphertext multiplications from 5 to 3. The one-plaintext-multiplication saving came from rewriting c6·T6 + c4·T4 as T4·(2·c6w + c4) − c6w, which also let the two terms meet at different levels.

The remaining plaintext additions and multiplications cannot be merged. Doing so would need arithmetic between plaintexts, which CKKS here does not allow.

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
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;
  Ciphertext w2 = w * w;
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext c6w = w * c6;
  Ciphertext q = c6w + c6w + c4;
  Ciphertext p = t4 * q;
  Ciphertext term2 = w * c2;
  Ciphertext term1 = z * c1;
  Ciphertext r = term2 - c6w;
  r = r + term1;
  Ciphertext result = p + r;
  result = result + c0;
  result.set_output("y");
}
```