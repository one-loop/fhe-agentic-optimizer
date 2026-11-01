Idea: only even powers matter except T1. Write the polynomial in z: with u = z², the even part c0 + c2T2 + c4T4 + c6T6 is a cubic in u, and the odd part is just c1·z. This removes most of the multiplications.

T2 = 2u−1
T4 = 8u²−8u+1
T6 = 32u³−48u²+18u−1

Result = c0 + c1 z + c2(2u−1) + c4(8u²−8u+1) + c6(32u³−48u²+18u−1)
= A0 + A1 u + A2 u² + A3 u³ + c1 z

The coefficients A_i are plaintext combinations, and I can't compute them with plaintext-plaintext arithmetic. Constants are only available as plaintext inputs, so every coefficient multiply needs a ciphertext operand. I'll therefore keep the c_k multiplications on ciphertexts.

Cheaper plan: u = z², and u = x²·alpha², so I use z = x·alpha, then u = z*z (one ct-ct multiply).
Then with T2 = 2u−1, T4 = 2T2²−1, T6 = 2·T2·T4 − T2:
- T2 = u+u−one (depth 2)
- T2² (a square), T4 = s+s−one
- T6 = 2·T2·T4 − T2 (another multiply)

That's 4 ct-ct multiplies (u, T2², T2·T4, plus z), versus 5 in the baseline. Alternatively, use T6 = 2T3² − 1 with T3 = 4z³−3z, which is no better.

Reduce the plaintext multiplies: the sum c2·T2 + c4·T4 + c6·T6 + c1·z needs 4 plaintext multiplies, same as the baseline. The depth is lower, though: z (1), u (2), T2 (2), T4 (3), T6 (4), then the coefficient multiply (5), so 5 levels against 7.

To avoid scale mismatches, the terms will meet at different levels, and CHEHAB aligns them.

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
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```