Idea: write the even part in terms of t2 = T2 = 2u−1, where u = z². Then T4 = 2t2²−1 and T6 = 4t2³−3t2, so

y = c1·z + (c2−3c6)·t2 + 2c4·t2² + 4c6·t2³ + (c0−c4).

I build the combined coefficients from the ciphertext side: w = c6·t2, and the constant multiples come from additions. This needs only 3 ct-ct multiplications (u, t2², w·t2²) instead of 5, and the depth drops to 4.

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
  Ciphertext w = t2 * c6;
  Ciphertext q = w * s;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext a = s * c4;
  Ciphertext a2 = a + a;
  Ciphertext b = t2 * c2;
  Ciphertext w3 = w + w + w;
  Ciphertext result = q4 + a2;
  result = result + b;
  result = result - w3;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```