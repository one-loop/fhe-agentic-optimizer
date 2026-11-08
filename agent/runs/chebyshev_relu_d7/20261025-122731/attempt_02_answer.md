Idea: reduce the multiplicative depth and plaintext multiplies further.

Use u = z² directly as (x·x)·(alpha²), but alpha² would be a plaintext-plaintext product, which is not allowed. Instead, fold alpha in differently: z = x·alpha costs one pt mul, then u = z·z costs one ct mul. Using square(z) is the same cost.

The even part is a polynomial in u. With P(u) = a0 + a1·u + a2·u² + a3·u³, the coefficients cannot be precomputed. Instead I use the structure T4 = 2·T2² − 1 and T6 = 2·T2·T4 − T2, so the even part is:

c0 + c2·T2 + c4·T4 + c6·T6 = c0 + T2·(c2 − c6) + c4·T4 + 2c6·T2·T4

Group: T6·c6 = (m + m − t2)·c6, where m = t2·t4. Rewrite it as m·(c6+c6) − t2·c6. This avoids one level: m is at level 4, and the multiply by c6 lands it at level 5 either way, so the depth is unchanged. But I can instead compute w = t4·c6 (level 4), then t2·w (ct mul, level 5), which is also depth 5.

A better saving: fold c4 and c6 together as t4·(c4 + 2c6·t2) − c6·t2, where:
- q = c6·t2 (pt mul, level 3)
- r = q + q (add)
- v = r + c4 (plaintext add; this is fine since r is a ciphertext)
- t4·v is one ct mul, at level 4 (t4 is at level 3, v at level 3)

This removes a ct-pt multiply: m is gone, and t6 is replaced by the t4·v product. Terms:
- t4·v is at level 4
- c1·z at level 2
- c2·t2 at level 3
- −q (reuse)

Result = c0 + c1·z + c2·t2 − q + t4·v. The depth drops to 4.

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
  Ciphertext q = t2 * c6;
  Ciphertext r = q + q;
  Ciphertext v = r + c4;
  Ciphertext w = t4 * v;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - q;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```