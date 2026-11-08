The scale mismatch came from adding a·b, which has three multiplications behind it, to a·c1, which has two, at the same level.

Fix: give the c1 term the same multiplication history by making it a product of the same shape. Write

y = c0 − c2 + z·(c1 + 2·c2·z)

with z = x·alpha:
- w = x·c2 (ct-pt, level 1)
- w2 = w + w
- v = w2·alpha, which gives 2·c2·z/… and needs one more level. That gets expensive.

A cheaper route is to build the linear factor first at level 1:
- u = x·alpha (level 1, one multiplication)
- s = x·c2 (level 1, one multiplication)
- q = u·s is a ct-ct mul at level 2 and equals c2·alpha·x² = c2·z²·(1/alpha). That is wrong, because alpha is not squared. Check: u·s = x²·alpha·c2 = 4·c2·z²·alpha² / alpha... Expressed in z (with alpha = 0.25, x = z/alpha): x²·alpha·c2 = z²·c2/alpha, which is not c2·z². So attempt 1 was also numerically wrong.

Correct version: use z = x·alpha, then z·z has the right form.
- z = x·alpha (level 1)
- g = z·c2 (level 2) and g2 = g + g
- h = z·c1 (level 2)
- the quadratic term is g2·z, a ct-ct mul at level 3. It would meet h only at different levels, and the mismatch rule is avoided if h is at level 2 and the quadratic is at level 3.

Alternatively, share the structure: m = c1 + g2 (a plaintext add into a ciphertext at level 2), then r = m·z (level 3), then r − c2 + c0. This uses ct-pt: alpha, c2 (2), ct-ct: 1. It has one fewer ct-pt multiplication than the baseline. Depth 3, the same as the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext g = z * c2;
  Ciphertext g2 = g + g;
  Ciphertext m = g2 + c1;
  Ciphertext r = m * z;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```