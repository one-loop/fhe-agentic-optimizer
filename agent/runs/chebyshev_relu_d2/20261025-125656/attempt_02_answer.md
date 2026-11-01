Idea: reduce depth and multiplies further. Currently z = x*alpha (pt mul), v = z*c2 (pt mul), then ct-ct multiply with z.

Instead, avoid computing z first. Let p = x*c2 (pt mul, level 1) and z = x*alpha (pt mul, level 1) run in parallel. Then y = z·(c1 + 2·α·p) + c0 − c2 still needs the extra α multiply.

Alternative: use x directly in the ct-ct multiply. With a = x*alpha, the quadratic is 2c2·a² = (a·c2')·(a)... The product of two level-1 ciphertexts gives depth 2 total:
- a = x*alpha (level 1)
- b = x*c2 (level 1)
- b2 = b + b
- q = a * b2 = 2·c2·α·x² (not 2·c2·α²·x²), missing a factor α.

Fix: let b = a*c2 would add a level. Instead, take q = a * a (ct-ct, level 2), then q*c2 gives depth 3 again. Another option is to make the linear coefficient absorb things: y = a·(c1 + 2c2·a) + c0 − c2, where the factor (c1 + 2c2·a) needs c2·a, which is at level 2 unless c2·x is used with alpha on the other side:
- u = x*c2 (level 1), a = x*alpha (level 1)
- u·a = c2·α·x² (ct-ct, level 2); it needs one more α, which is the same problem.

Alternative: use the linear term via x directly. Let d = x*c2 (level 1) and e = d*alpha (level 2), which equals c2·a, the same depth as before. So the ct-ct multiply is at level 3 regardless, unless I give up a plaintext multiply. Pairing instead: a = x*alpha (L1), and a·a (L2, ct-ct), then multiply by c2 (L3). The linear term would also be a·c1 at L1. Counts: 3 pt mul, no gain.

Try reducing cost within the same depth: compute w = (v2 + c1) as before, but fold the constants. Do the final multiply at a deeper level, which is cheaper, and put c0 − c2 as a single add of the ciphertext... Not possible without plaintext arithmetic. Instead, I can attempt a different arrangement: m = w*z, where z has a level mismatch with w (z at L1, w at L2), so CHEHAB inserts a mod switch. Use x*alpha computed twice? Not helpful.

An option: y = (c2·2a + c1)·a, computed as (x*c2 + x*c2)... Let k = x*c2 (L1) and then k2 = k+k; k2 * a gives 2c2·α·x² (L2); we are missing α. Multiply instead by alpha² ... no.

Try swapping the order: the ct-ct multiply of (k2·alpha) isn't possible without a level. I'll try the variant where the subtraction is merged: r = m + (c0 − c2) replaced with r = (m + c0) − c2 (already done). So I'll attempt a variant with 2·c2 formed by adding after the multiply and the ct-ct multiply of a and v:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = z * c2;
  Ciphertext w = v + c1;
  Ciphertext m = w * z;
  Ciphertext r = m + m;
  Ciphertext s = z * c1;
  r = r - m;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```

Wait, that is wrong: it computes m + m − m = m, i.e. (c2·z + c1)·z, which is not the target. Here is the correct version of the same idea: y = z·(2·c2·z + c1) + c0 − c2, with the doubling placed after the product of v and z (a ciphertext add replaces one add on a smaller operand):

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = z * c2;
  Ciphertext q = v * z;
  Ciphertext q2 = q + q;
  Ciphertext l = z * c1;
  Ciphertext r = q2 + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```