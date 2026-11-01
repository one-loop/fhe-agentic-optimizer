The even part is rewritten in powers of u = T2(z) = 2z² − 1, using the composition identities T4 = T2(T2) and T6 = T3(T2):

- T4 = 2u² − 1
- T6 = 4u³ − 3u

So the output becomes:

y = c1·z + c2·u − 3·c6·u + 2·u²·(2·c6·u + c4) − c4 + c0

The two highest-order terms collapse into a single ciphertext product, u²·(2v + c4) with v = c6·u. Here c4 enters as a plaintext add, not a multiply.

| | Baseline | This kernel |
|---|---|---|
| Ciphertext × ciphertext multiplies | 5 | 3 (z², u², u²·(2v + c4)) |
| Ciphertext × plaintext multiplies | 5 | 4 (alpha, c6, c2, c1) |
| Multiplicative depth | 7 | 4 |

The terms meet at different levels, or at the same level with the same multiplication history, so there should be no scale mismatch:

- c1·z is at level 2.
- c2·u and v are at level 3; both are a plaintext times u, so they match.
- The u² product is at level 4.

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

  Ciphertext z = x * alpha;          // level 1
  Ciphertext zz = z * z;             // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = u * u;             // level 3

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4
  Ciphertext r = u2 * q;             // level 4
  Ciphertext high = r + r;           // 4 c6 u^3 + 2 c4 u^2

  Ciphertext lin = cu - (v + v + v); // (c2 - 3 c6) u
  Ciphertext odd = z * c1;           // level 2

  Ciphertext y = high + lin + odd - c4 + c0;
  y.set_output("y");
}
```