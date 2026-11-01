This is the last attempt, and I am resubmitting attempt 1 unchanged as the final kernel. Every alternative structure I tried or reasoned through is either normalized by CHEHAB into this same circuit or costs more.

The kernel uses Horner form: y = z·(2·c2·z + c1) + (c0 − c2), with z = x·alpha. Each part is at its minimum:

- **Ciphertext multiply:** one, done at level 2.
- **Plaintext multiplies:** two scalar ones, by alpha and by c2. Every other split needs at least three.
- **Depth:** 3, which is the minimum because the x² coefficient needs the product c2·alpha·alpha.
- **Constant adds:** three. Plaintext-only combinations such as c0 − c2 are not allowed, so they can't be merged.
- **Alignment:** one mod switch of about 1 ms. Removing it costs more, as attempt 6 measured: 46.36 ms against 41.52 ms.

The result is 41.52 ms against the baseline's 53.55 ms (×1.29 faster).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext w = z * c2;             // level 2: c2*z
  Ciphertext inner = w + w + c1;     // 2*c2*z + c1
  Ciphertext prod = inner * z;       // level 3
  Ciphertext result = prod + c0 - c2;
  result.set_output("y");
}
```