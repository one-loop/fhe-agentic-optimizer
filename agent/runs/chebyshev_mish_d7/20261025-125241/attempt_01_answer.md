Idea: convert the Chebyshev series into a power-basis polynomial in x, so the plaintext coefficients can be combined cheaply. The conversion is not possible at compile time, because the coefficients are plaintext inputs. Instead I use an evaluation scheme with few ct-ct multiplications and a low depth.

With z = alpha*x, the polynomial is p(z) = c0 + c1 z + ... + c7 T7(z). The Chebyshev products are the expensive part.

Plan using the product identities T_{m+n} = 2 T_m T_n - T_{|m-n|}:
- z2 = z*z, T2 = 2z2 - 1 (depth 2 after the alpha multiply).
- T3 = 2 z T2 - z, T4 = 2 T2² - 1.
- T4 needs T2² (depth 3).
- T5 = 2 T2 T3 - z, T6 = 2 T3² - 1, T7 = 2 T3 T4 - z.

That is 6 ct-ct multiplies (z², zT2, T2², T2T3, T3², T3T4), the same count as the baseline but with lower depth. T3 sits at depth 3 and T4 at depth 3, so T7 lands at depth 4. Each ct-pt coefficient multiply adds one more level, giving depth 5 against 8.

Reducing multiplies further: to avoid the separate alpha multiply, I compute z = x*alpha (one plaintext multiply). The coefficient multiplies are 7 ct-pt, with c1 folded into the z term.

The scale subtraction rules need care: "2*z2 - one" subtracts a plaintext, which is fine. For T3 = 2 z T2 - z, z is at a shallower level, and CHEHAB aligns it. The sums of the c_k terms are at different levels, which is fine.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext s3 = square(t3);
  Ciphertext t6 = s3 + s3 - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```