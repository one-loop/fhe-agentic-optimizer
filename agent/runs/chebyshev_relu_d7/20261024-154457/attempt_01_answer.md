Idea: the polynomial only uses even terms c2, c4, c6, c0 plus the odd term c1·z. Writing everything in terms of u = z², the even part is a polynomial in u of degree 3, and the single odd term is c1·z.

T2 = 2u − 1
T4 = 8u² − 8u + 1
T6 = 32u³ − 48u² + 18u − 1

The even part is c0 + c2(2u−1) + c4(8u²−8u+1) + c6(32u³−48u²+18u−1) = a0 + a1·u + a2·u² + a3·u³, where the a's are combinations of the coefficients. Those combinations need plaintext-plaintext arithmetic, which is not allowed. I can only form them by multiplying a ciphertext by plaintexts, so I keep the coefficients as plaintext multipliers on ciphertext terms.

Cheaper scheme:
- z = x·alpha (1 plaintext multiplication).
- u = z² (1 ciphertext multiplication).
- u2 = u² (1 ciphertext multiplication).
- T2 = u+u−one, T4 = 8u2 − 8u + one. Multiples of 8 come from additions: 8a is three doublings.
- T6 = 32u³ − 48u² + 18u − 1 = T2·(T4 stuff). It is simpler to use T6 = 2·T2·T4 − T2, which needs one ciphertext multiplication, T2·T4.

Total ciphertext multiplications: z² (1), u² (1), T2·T4 (1) = 3, down from 5. Plaintext multiplications: alpha, c1·z, c2·T2, c4·T4, c6·T6, which is 5, the same as the baseline.

Depth: z is level 1, u level 2, T4 level 3, T6 level 4, then times c6 gives level 5. This is lower than 7.

To cut the plaintext multiplications, I fold alpha into c1: c1·z = (x·alpha)·c1 is already needed as z for the square, so I reuse z. The scale alignment is handled by CHEHAB.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext t2t4 = t2 * t4;
  Ciphertext t6 = t2t4 + t2t4 - t2;
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```

Note that I used T4 = 2·T2² − 1 (the doubling identity), which is mathematically equivalent and needs the same single squaring of T2.