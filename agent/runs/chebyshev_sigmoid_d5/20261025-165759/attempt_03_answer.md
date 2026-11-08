Idea: use t = 4s − 3, with s = z². Then T3/z = t and T5/z = 16s² − 20s + 5 = t² + t − 1. So

P = c1 + c3·t + c5·(t² + t − 1) = t·(c5·t + c3 + c5) + c1 − c5, and y = z·P + c0.

This needs only 2 ct-ct multiplies, 1 square and 2 pt multiplies (z and c5·t), against 3 ct-ct, 1 square and 3 pt in the best kernel. It also needs far fewer adds.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext t = s4 - one;
  t = t - one;
  t = t - one;
  Ciphertext m = t * c5;
  Ciphertext r = m + c3;
  r = r + c5;
  Ciphertext P = r * t;
  P = P + c1;
  P = P - c5;
  Ciphertext y = z * P;
  y = y + c0;
  y.set_output("y");
}
```