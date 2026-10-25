Idea: only even terms (c2, c4, c6) and the odd term c1 matter, so the polynomial is c0 + c1 z + c2 T2 + c4 T4 + c6 T6. Since c3, c5 and c7 are dropped, the odd part is just c1 z.

Writing u = z², the even part is a polynomial in u of degree 3:
- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

Since no numeric constants are allowed, the coefficients must come from the plaintext inputs. Collect the coefficients of the even part, with u = z² = x²·alpha²:

E(u) = c0 + c2(2u−1) + c4(8u²−8u+1) + c6(32u³−48u²+18u−1)

Writing E = a0 + a1 u + a2 u² + a3 u³ would need plaintext-plaintext arithmetic, which is forbidden. Instead I can use ciphertext sums with plaintext multiplies:
- a3 = 32 c6
- a2 = 8 c4 − 48 c6
- a1 = 2 c2 − 8 c4 + 18 c6
- a0 = c0 − c2 + c4 − c6

Each of these needs the coefficients combined, which means multiplying plaintexts together. That is not possible directly, but doubling can be done by self-addition on a ciphertext. So I use the structure with ciphertext intermediates.

Cheaper plan:
- u = (x·x)·alpha². Computing x² first and then multiplying by alpha twice costs more. Instead, z = x·alpha (a plaintext mult) and u = square(z).
- The T_k terms can be built from u alone: T2 = 2u − 1, T4 = 2T2² − 1, T6 = 2·T2·T4 − T2... that is still multiplications.

Alternative using T2 = w: T4 = 2w² − 1 and T6 = 4w³ − 3w. This gives
- c2 w + c4(2w² − 1) + c6(4w³ − 3w)
= w(c2 − 3c6 + 4c6 w²) + 2c4 w² − c4.

I can't form c2 − 3c6 without plaintext arithmetic, so I use ciphertext multiplies by the plaintexts and add them:
- Let w2 = w², so the sum is c2·w + c4·(w2+w2) − c4·1 + c6·(T6).
- T6 = w·(w2+w2+w2+w2 − 3), which is w·(4w2 − 3). That is one more ct-ct multiplication, and the −3 constant is a problem.
- Use T6 = 2·w·T4 − w, with T4 = (w2+w2) − one. Then T6 = 2·w·T4 − w.

Multiplication count: z (pt), u = square(z), w = 2u − one, w2 = square(w), T4 = 2w2 − one, w·T4 (ct-ct). That is 3 ct-ct multiplications, down from 5. Depth: z 1, u 2, w2 3, w·T4 4. The baseline depth was 7.

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
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext wt4 = w * t4;
  Ciphertext t6 = wt4 + wt4 - w;
  Ciphertext result = z * c1;
  result = result + w * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```