Idea: remove a ct-ct multiply by getting u = z² with fewer levels, and merge the linear terms.

The current best has 3 ct-ct muls (z·z, t2·t2, t4·v) and 4 pt muls (alpha, c1, c2, c6). Options to shed work:

1. Merge the c1 and c2 terms into one pt mul. The odd term c1·z is at level 1, and c2·t2 at level 2. They cannot be merged directly. But c1·z can be computed as x·(alpha·c1), and alpha·c1 is a plaintext-plaintext product, which is not allowed. So that is not possible.

2. Avoid the separate pt mul for c2. Fold it into the t2 product: t2·c2 − q = t2·(c2 − c6), and c2 − c6 is also a plaintext-plaintext operation. Instead write t2·c2 − t2·c6 as one multiply by using the structure v = 2q + c4. Then result = c0 + c1·z + t2·c2 − q + t4·v. Let t2·(c2 − c6) be expressed via the ciphertext p = c2 − c6, but that cannot be built without a ciphertext. Skip it.

3. Drop the level-1 alignment cost. z·c1 sits at level 2 while the other terms are at level 3 or 4. CHEHAB aligns with a mod switch, which is cheap.

4. Compute u = z² as a square of z, saving relinearization cost slightly: square(z) is counted as a ct-ct multiplication, but may be cheaper in practice. t2·t2 can also be written as square(t2).

I take option 4 (square), which changes the circuit's operation counts (square instead of ct_ct_mul) and may be a bit faster.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
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