Idea: cut the multiplies further and reduce depth.

- Use u = x² directly (unscaled) instead of z², so the squaring starts at level 0 without the alpha multiply in front. z² = alpha²·x², but alpha² is not available as a plaintext without plaintext-plaintext arithmetic. So I use u' = (x*alpha)*(x*alpha), which is the same cost as before. Instead I square z and skip the separate z*c1 chain: odd term is x*alpha*c1, which needs z anyway.
- Remove the extra plaintext multiply for t6: c6*T6 = c6*(2·w − t2) = 2·(w*c6) − t2*c6. This saves nothing in multiplies, so I leave it.
- Merge the even terms: c2*T2 + c4*T4 + c6*T6 with T4 = 2s − 1, T6 = 2w − T2 gives c2*T2 − c6*T2 + 2c4*s + 2c6*w − c4 + .... This needs c2−c6, which is plaintext arithmetic and not allowed. Rather, I group by ciphertext: (t2*c2 − t2*c6) needs two multiplies, so no gain.
- A real gain: T6 = 2·T4·T2 − T2 costs a ct-ct multiply at depth 4. Alternatively, with s = T2², w = T4·T2 = (2s−1)·T2 = 2·s·T2 − T2, so no gain either.

So I keep the structure of attempt 1 and cut the plaintext multiplies for odd and even parts by reusing products. The per-term c_k multiplications are 4 (z·c1, c2, c4, c6). Using c4*t4 = c4*(2s − 1) = (s*c4) + (s*c4) − c4 does not reduce the count, but it moves the multiply to a shallower ciphertext (s is a level above t4's addition, so there's no real difference). I also apply this to t6: w*c6 doubled, minus t2*c6, and combine t2*c6 with t2*c2 via a single multiply of t2 by a shared plaintext, which is not available.

I found no safe further reduction, so this attempt makes a small change: compute the c-multiplies on the raw products (s, w, u) and double afterwards, which lets these multiplies run in parallel with the deeper chain and at deeper levels.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext w = t4 * t2;
  Ciphertext sc = s * c4;
  Ciphertext wc = w * c6;
  Ciphertext uc = u * c2;
  Ciphertext t2c = t2 * c6;
  Ciphertext result = z * c1;
  result = result + (uc + uc);
  result = result + (sc + sc);
  result = result + (wc + wc);
  result = result - t2c;
  result = result - c4;
  result = result - c2;
  result = result + c0;
  result.set_output("y");
}
```