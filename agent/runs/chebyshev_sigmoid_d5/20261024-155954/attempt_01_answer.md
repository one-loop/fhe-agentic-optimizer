Idea: write the odd polynomial directly. With z = alpha·x, the result is c0 + z(c1 + c3(4z²−3) + c5(16z⁴−20z²+5)), which folds into a polynomial in u = z²:
y = c0 + z·(a + b·u + d·u²), with a = c1 − 3c3 + 5c5, b = 4c3 − 20c5, d = 16c5.

The problem is that constants can't be formed from plaintexts without a ciphertext operand, so a, b, d can't be computed as plaintext-plaintext arithmetic. Instead I fold the coefficients into the ciphertext side and keep the constant multiplies minimal.

Using only the available inputs:
- z = x·alpha (1 pt mul)
- u = z·z (1 ct mul)
- Odd form: y = c0 + c1 z + c3 T3 + c5 T5, with T3 = z(4u−3) and T5 = z(16u²−20u+5).
- Write T3 = 4zu − 3z, so everything is a combination of z, zu and zu². The coefficient multiplications still need c-scaled terms, and without constants I cannot make 4c3 directly. Doubling by addition is allowed, though: 4c3·(zu) = (c3·zu) doubled twice.

Plan:
- p = c3·z (pt mul at the z level)
- q = c5·z (pt mul)
- Let w = u·(stuff), where stuff = 4p − 20q + ... is built from additions, so the c1/c3/c5 combination happens before multiplying by u.
- Define A = c1·z − 3c3·z + 5c5·z. The scalars multiply z once each and then combine by additions: c1z + c3z·(−3) = c1z − (p+p+p) and so on. That costs three pt muls on z.
- B = 4c3 − 20c5 applied to z: B·z = 4p − 20q = 4(p − 5q) = 4(p − q − 4q), all done by additions.
- D = 16c5 z = 16q, by four doublings.
- y = c0 + A + u·(Bz) + u²·(Dz) = c0 + A + u·(Bz + u·Dz).

Multiplications: z (pt), c1z, p, q (3 pt muls), u (ct), u·(Bz + u·Dz) (2 ct), which makes 3 ct muls. This is fewer than the baseline's 4 ct muls. The two ct muls are inner = Bz + u·Dz, then u·inner, which is sequential, so the depth is fine.

Scale: Bz and u·Dz are at different levels, which CHEHAB aligns. Bz carries one pt mul on top of z, and u·Dz has one more ct mul, so they differ in level and are fine.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext p3 = p + p + p;
  Ciphertext q5 = q + q + q + q + q;
  Ciphertext A = c1z - p3 + q5;
  Ciphertext q4 = q + q;
  q4 = q4 + q4;
  Ciphertext pq = p - q5;
  Ciphertext B = pq + pq;
  B = B + B;
  Ciphertext D = q4 + q4;
  D = D + D;
  Ciphertext inner = B + u * D;
  Ciphertext y = A + u * inner;
  y = y + c0;
  y.set_output("y");
}
```