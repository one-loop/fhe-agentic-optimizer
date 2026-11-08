Idea: cut the plaintext multiplies and the depth by using x directly for the odd term and merging terms before multiplying.

Current best: z=x·alpha (pt), u=z² (ct), t2, s=t2², t4, q=t2·c6, w=t4·v, plus z·c1 and t2·c2 (pt). That makes 4 pt muls and 3 ct muls.

Change: merge the c2 and c6 plaintext multiplies on t2, with no plaintext-plaintext arithmetic needed.
- Even part = c0 + c2·t2 + c4·t4 + c6·t6. In terms of t2, t4 = 2t2² − 1 and t6 = t2·(2·t4 − 1) = 4t2³ − 3t2.
- So E = c0 − c4 + t2·(c2 − 3c6) + 2c4·t2² + 4c6·t2³.
- Factor it as t2·[(c2 − 3c6) + t2·(2c4 + 4c6·t2)]. That still needs c2 − 3c6, which would be a plaintext-plaintext operation.
- Instead, avoid c2 by taking the product with t2 on the ciphertext side: let g = (c2·t2 − 3q) is a ciphertext (pt muls on t2).
- That is still the same pt mul count, so no gain there.

A different saving: z·c1 and z = x·alpha. Replace the odd term with (x·c1)·alpha, which has the same cost. Instead, share the alpha multiply: x·c1 is a pt mul at level 1, and u = z² needs z anyway. So there is no saving.

Another option is to compute u = z² as (x·x) scaled by alpha, which requires alpha² and is unavailable. But u·alpha can be applied afterwards: x² is a ct mul at level 1 (x is fresh), and then t2 = 2·alpha²·x² − 1 needs alpha twice. Writing (x·alpha)·(x·alpha) is already the cheapest form.

What I can do is reduce the depth of the final merge. Make v cheaper by taking t4·v with v at level 3 while t4 is at level 3, which matches. The remaining change is to compute the c2 term from u rather than t2: c2·t2 = 2c2·u − c2, so c2·u is a pt mul at level 2 (one level earlier than t2·c2, hence cheaper because u is at a shallower level). The same holds for q: use ua = u·c6, and then q = 2ua − c6. That makes the pt muls run on u at a higher level, which is cheaper, and the products land at the same level as before. I'll try that.

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
  Ciphertext ua = u * c6;
  Ciphertext q = ua + ua;
  q = q - c6;
  Ciphertext r = q + q;
  Ciphertext v = r + c4;
  Ciphertext w = t4 * v;
  Ciphertext ub = u * c2;
  Ciphertext t2c2 = ub + ub;
  t2c2 = t2c2 - c2;
  Ciphertext result = z * c1;
  result = result + t2c2;
  result = result - q;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```