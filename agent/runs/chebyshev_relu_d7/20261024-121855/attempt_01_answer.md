Idea: only even terms c2, c4, c6 plus odd c1 are needed. Write the polynomial in powers of z directly: p(z) = c0 + c1 z + c2 T2 + c4 T4 + c6 T6. T2, T4, T6 are polynomials in u = z², so the even part is E(u) = a0 + a1 u + a2 u² + a3 u³, where the coefficients are combinations of c's. Those combinations need plaintext-plaintext arithmetic, which is not allowed. Instead I keep the Chebyshev forms but compute them with few multiplications:

- u = z² (z = x*alpha costs one plaintext multiply). T2 = 2u − 1.
- T4 = 2 T2² − 1, and T6 = 2 T2 T4 − T2 (T6 = 2 T4 T2 − T2 indeed, since T_{n+m} + T_{n-m} = 2 T_n T_m, with n=4, m=2).
- Depth: z (1), u (2), T2 (2), T4 (3), T6 (4), and the c6 multiply brings it to 5. This is fewer levels than 7.

Multiplications: z*z, T2*T2, T4*T2, which is 3 ct-ct (baseline has 5). Odd part: c1*z is a plaintext multiply. To cut the multiplications further, I fold alpha into the plaintext multiplies where it is free: c1*z = x*alpha*c1 would need a plaintext product, so I use (x*alpha) once and reuse it.

Even part: c2*T2 + c4*T4 + c6*T6 + c0, three plaintext multiplies. The `one` input is only used as a subtractor: T2 = (u+u) − one, T4 = (s+s) − one where s = T2*T2, T6 = (w+w) − T2 where w = T4*T2.

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
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext w = t4 * t2;
  Ciphertext t6 = w + w - t2;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```