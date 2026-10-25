Idea: y = c0 + c1·z + c2·(2z² − 1) with z = αx, which expands to y = c0 − c2 + c1·α·x + 2·c2·α²·x².

Plaintext-plaintext arithmetic is not allowed, so I cannot precompute c1·α or c2·α². I can fold them in on the ciphertext side instead:
- a = x * (c1·α) is not available, but u = x * alpha gives z, and (c1·z) is one ct-pt multiply.
- z² · c2 would cost an extra level. Better: w = z * c2 (ct-pt), then w * z (ct-ct) = c2·z². Its level history is the same as c1·z followed by one more multiply, so the two terms still need alignment.

Rewrite: y = c0 − c2 + z·(c1 + 2·c2·z) = c0 − c2 + z·(c1 + w + w), with w = c2·z.
- z = x·alpha (ct-pt, level 1)
- w = z·c2 (level 2)
- s = w + w + c1 (plaintext add, level 2)
- z·s (ct-ct, level 3)

This still has 3 levels, and the multiplies are 2 ct-pt and 1 ct-ct. Alternatively, with p = x·c2 (ct-pt) and q = x·alpha, the product p·q = c2·α·x² has depth 2 but needs a further factor of α. Using x·alpha twice gives the same count, so I'd rather cut the cost differently.

Option: t = x·alpha (L1), then (t·t) is ct-ct at L2. The c2 factor can be applied to one operand before the multiply: m = t·c2 (L2), and m·t (L3). The 2·c2 factor is a doubling by addition, which is free. The c1 term: t·c1 sits at L2, then added to a level-3 term, which CHEHAB aligns.

Total: 2 ct-pt multiplies for t and m, 1 ct-ct multiply, and 1 more ct-pt multiply for c1·t. That is the baseline minus one multiply (the baseline has 3 ct-pt). I'll use the Horner form, which merges the c1 term into the product and drops the extra multiply:

y = (c0 − c2) + z·(c1 + 2·w), where the constant c0 − c2 must be built as a ciphertext-side expression. I can do (z·s) + c0 and then subtract c2, which is a plaintext add/sub with a ciphertext operand, so it is allowed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext s = w + w;
  s = s + c1;
  Ciphertext r = z * s;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```