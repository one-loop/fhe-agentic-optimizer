Write y = c0 + c1·z + c2·(2z² − 1), with z = alpha·x.
This gives y = c0 − c2 + c1·alpha·x + 2·c2·alpha²·x².

Only the plaintext inputs are available, so I can't precompute constants such as a = c1·alpha directly (no plaintext-plaintext arithmetic). Instead I factor the polynomial as a product of linear terms in x:

y = c0 − c2 + x·(c1·alpha + 2·c2·alpha²·x)

The inner coefficients still require plaintext products. Alternatively, use z = x·alpha (one ct-pt mul), then:
- u = z·(c1 + 2·c2·z) needs 2·c2 formed from c2 + c2, which is plaintext-plaintext and not allowed.

Instead, let w = z·c2 (ct-pt), then w2 = w + w, and u = c1 + w2 (plaintext add). Then z·u is a ct-ct mul, giving c1·z + 2·c2·z². The constant is c0 − c2, which needs plaintext subtraction, so I fold it in as ciphertext operations: result − c2 + c0 (two plaintext adds/subs).

Operation counts:
- Baseline: 3 ct-pt muls, 1 ct-ct mul.
- Mine: z = x·alpha (1 ct-pt), w = z·c2 (1 ct-pt), then a ct-ct mul. That is 2 ct-pt muls and 1 ct-ct mul, but depth 3.

A cheaper variant is to compute x·c2 (one ct-pt mul) in parallel with x·alpha, then multiply the two, which gives depth 2:
- a = x·alpha, b = x·c2, p = a·b = alpha·c2·x², and p2 = p + p. This equals c2·z², so 2·c2·z² = p2.
- The c1 term is a·c1 (ct-pt), taking the same level as p2.
- The result is p2 + a·c1 − c2 + c0.

That is 3 ct-pt muls and 1 ct-ct mul, the same as the baseline but with depth 2. Better: a·c1 can be merged as a·(c1 + b2), but that is again a ct-ct mul with depth 3. I'll go with the depth-2 version, which is cheaper because the later ops run at deeper levels. The scale of a·c1 matches that of a·b (both are a ct-pt mul after a ct-pt mul vs. a ct-ct mul), so they should align.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext a = x * alpha;
  Ciphertext b = x * c2;
  Ciphertext p = a * b;
  Ciphertext p2 = p + p;
  Ciphertext t1 = a * c1;
  Ciphertext r = p2 + t1;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```