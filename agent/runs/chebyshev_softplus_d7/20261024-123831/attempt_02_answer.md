Idea: cut the number of multiplies and levels further by working with a polynomial in u = z² directly.

The result is c1·z + c0 + c2·T2 + c4·T4 + c6·T6. In terms of u, this is a cubic: a0 + a1·u + a2·u² + a3·u³. Forming a_j from the c's needs plaintext-plaintext arithmetic, which is not allowed. But the multiplies by plaintexts can be distributed: each term c_k·T_k is a ct-pt multiply anyway.

Use the identities T4 = 2w²−1 and T6 = 2·w·T4 − w, with w = T2. Then:
- c4·T4 = 2·c4·w² − c4
- c6·T6 = 2·c6·w·T4 − c6·w = w·(2·c6·T4 − c6)

So the sum is c0 + c1·z + c2·w + c4·(2w²−1) + c6·w·(2T4−1).
Group by w: c4·T4 + w·(c2 + c6·(2T4−1)) is still deep.

A simpler way to save work is to reduce the pt multiplies on the lower-depth terms. Here z·c1 and w·c2 are computed early, at shallow levels, so they are cheap and sit at different levels than the deep terms. The ct-ct multiplies are the main cost, and there are only three of them (u, w², w·t4). Since u = z² can be computed as square(z), I keep that.

An improvement that avoids a level: compute t4 from w² and fold c6 earlier, as c6·T6 = (c6·w)·(2·t4 − one). Here c6·w is computed in parallel with t4, so the ct-ct multiply lands at the same depth, and no final pt multiply is needed for the t6 term. This removes one level from the critical path and one pt multiply.

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
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext cw6 = w * c6;
  Ciphertext s = t4 + t4 - one;
  Ciphertext t6term = cw6 * s;
  Ciphertext result = z * c1;
  result = result + w * c2;
  result = result + t4 * c4;
  result = result + t6term;
  result = result + c0;
  result.set_output("y");
}
```