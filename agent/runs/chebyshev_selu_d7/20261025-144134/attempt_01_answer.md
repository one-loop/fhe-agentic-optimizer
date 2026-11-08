Idea: evaluate the polynomial in the power basis, so the c_k are folded into a plain polynomial in z with fewer ct-ct multiplies. The coefficients are only available as plaintext inputs, so I can't precompute the power-basis coefficients without plaintext-plaintext arithmetic. I therefore keep the Chebyshev form and reduce the cost another way.

The baseline computes z = x*alpha (a ct-pt multiply), then t2..t7 with 6 ct-ct multiplies, then 7 ct-pt multiplies. To cut depth and multiplies, use the doubling identities T2k = 2 Tk^2 - 1 and T(2k+1) = 2 Tk T(k+1) - z:
- z: depth 1
- t2 = 2z² - 1: depth 2
- t3 = 2 z t2 - z: depth 3
- t4 = 2 t2² - 1: depth 3
- t5 = 2 t2 t3 - z: depth 4
- t6 = 2 t3² - 1: depth 4
- t7 = 2 t3 t4 - z: depth 4

This is still 6 ct-ct multiplies but with lower depth, so the later operations run at deeper levels and are cheaper. The ct-pt multiplies by c_k come after each T_k, and the lowest-depth terms can be summed at different levels without a scale mismatch.

To save more, fold the constant `one` handling: -1 is needed as a plaintext subtraction, which is cheap (an add).

The final level is depth 4 plus 1 for the c_k multiply, giving 5 instead of 8.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
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