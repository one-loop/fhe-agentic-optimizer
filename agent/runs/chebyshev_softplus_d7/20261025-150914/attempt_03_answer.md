Idea: compute the c6 and c2 terms from w instead of t2. Then r = 2·(w·c6) − c6, and u = 2r + c4 is ready at depth 2, so it sits ready while t4 is computed. The multiply counts are unchanged, but the plaintext multiplies run one level higher up the chain, which should be a little cheaper and need less alignment.

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
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext wc6 = w * c6;
  Ciphertext r = (wc6 + wc6) - c6;
  Ciphertext u = (r + r) + c4;
  Ciphertext m = t4 * u;
  Ciphertext wc2 = w * c2;
  Ciphertext q = (wc2 + wc2) - c2;
  Ciphertext result = z * c1 + q - r + m + c0;
  result.set_output("y");
}
```