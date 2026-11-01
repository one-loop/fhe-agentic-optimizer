The kernel is unchanged across attempts 1–3, and the timing has stayed at about 41.3–41.4 ms, so the differences are measurement noise. I'm not going to resubmit it a fourth time without a real change, because I don't have a structurally different variant that would beat it.

Where the time goes (approximate, from the cost table):
- One ciphertext×ciphertext multiply: about 30 ms. This is the floor, since a degree-2 polynomial in x needs one x·x-type product.
- Two scalar plaintext multiplies (alpha, c2): about 5 ms each.
- Three plaintext additions (c1, −c2, c0): about 2.5 ms each.
- One mod switch: about 1 ms.

Ideas I considered and rejected:
- Dropping the mod switch by aligning `z` costs an extra scalar multiply, which is more than the 1 ms saved.
- Folding `c0 − c2` into one plaintext is plaintext-plaintext arithmetic, which is forbidden.
- Removing the `c1` addition needs another ciphertext multiply (about 30 ms).
- Evaluating in x instead of z needs an extra scalar multiply to form `c2·alpha`.

The remaining overhead above the 30 ms floor is about 11 ms, and none of the options above removes it without adding more. I'll keep the attempt-1 kernel as the answer.

```cpp
// Horner form of the degree-2 Chebyshev approximation in z = x/4:
//   y = c0 - c2 + z * (2*c2*z + c1)
// One ciphertext-ciphertext multiply, two scalar plaintext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");

  Ciphertext z = x * alpha;          // z in [-1, 1], depth 1
  Ciphertext a = z * c2;             // c2 * z, depth 2
  Ciphertext b = a + a;              // 2 * c2 * z (same level, no alignment)
  Ciphertext inner = b + c1;         // 2 c2 z + c1, depth 2
  Ciphertext p = inner * z;          // z * (2 c2 z + c1), depth 3
  Ciphertext q = p - c2;             // subtract c2
  Ciphertext y = q + c0;             // add c0
  y.set_output("y");
}
```