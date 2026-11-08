Idea: switch to a Horner-style form in t2. Since T4 = 2·t2² − 1 and T6(z) = T3(t2), the even part is P = c0 − c4 + (c2 − 3c6)·t2 + 2c4·t2² + 4c6·t2³. This reuses the s = t2² square. It also drops t4, so there is no `one` subtraction for it. The coefficients enter through w = c6·t2, using ciphertext adds for the 3× and 4× scalings. The final product is s·q with q = 2(2w + c4), and the leftover −c4 and +c0 are plaintext add/sub.

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
  Ciphertext v = w + w + c4;
  Ciphertext q = v + v;
  Ciphertext m = s * q;
  Ciphertext w3 = w + w + w;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - w3;
  result = result + m;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```