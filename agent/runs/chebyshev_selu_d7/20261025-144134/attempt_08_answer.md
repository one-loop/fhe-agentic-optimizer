Idea: same structure as attempt 7, with the plaintext adds placed at the shallowest terms. `c0` is added to the level-2 z-term (`z*c1 − z*c7`) instead of at the end. The lower sum is then accumulated shallow to deep, so each ciphertext add pairs a shallower term with a deeper accumulator and needs less alignment. The algebra is unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext t3 = z * (t2 + t2 - one);
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;

  Ciphertext p6 = t2 * c6;
  Ciphertext q = z * c5;
  q = q + p6;
  q = q + t3 * c7;
  Ciphertext w = q + q + c4;
  Ciphertext prod = t4 * w;

  Ciphertext low = (z * c1 - z * c7) + c0;
  low = low + (t2 * c2 - p6);
  low = low + (t3 * c3 - t3 * c5);
  Ciphertext result = prod + low;
  result.set_output("y");
}
```