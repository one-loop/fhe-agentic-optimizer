The previous variations did not change the circuit enough. This version switches to the Chebyshev basis in a = T2 = 2z² − 1, writing T_k = z·B_k with:
- B3 = 2a − 1
- B5 = 2a·B3 − 1
- B7 = 2a·B5 − 2a + 1

Then y = z·(c1 + c3·B3 + c5·B5 + c7·B7) + c0. It keeps 4 ct-ct multiplies but needs only 4 plaintext multiplies (c1 becomes a plaintext add) and far fewer ciphertext adds. The coefficient multiplies land at distinct levels.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext a = u + u - one;
  Ciphertext a2 = a + a;
  Ciphertext b3 = a2 - one;
  Ciphertext p5 = a2 * b3;
  Ciphertext b5 = p5 - one;
  Ciphertext p7 = a2 * b5;
  Ciphertext b7 = (p7 - a2) + one;

  Ciphertext q = b3 * c3;
  q = q + b5 * c5;
  q = q + b7 * c7;
  q = q + c1;
  Ciphertext r = z * q;
  r = r + c0;
  r.set_output("y");
}
```