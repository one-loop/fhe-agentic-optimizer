Attempt 6 is the best so far at 161.32 ms. Its cost is mostly fixed: 5 ciphertext multiplications, 6 plaintext multiplications, 3 alignments and about 8 plaintext adds.

This attempt keeps the same factorization but regroups the odd part so T4 multiplies the inner sum directly:

y = c0 + (c2 − c6)·T2 + T4·E + z·(R + 2·T4·O)

with E = c4 + 2c6·T2, O = c5 + c7·W and R = c1 − c7 + (c3 − c5)·W.

What changes compared with attempt 6:
- **Products:** z·O and the final T4 multiply are replaced by T4·O and z·(R + 2·T4·O). T4·E becomes its own product at a shallower level.
- **Joins:** the joins between levels move to different places.
- **Counts:** the number of ciphertext multiplications (5), plaintext multiplications (6) and alignments (3) stays the same.

I expect this to be about as fast as attempt 6; any gain is uncertain. It has not been run yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext T2 = zz + zz - one;
  Ciphertext W = T2 + T2 - one;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext E = (c6T2 + c6T2) + c4;
  Ciphertext O = W * c7 + c5;
  Ciphertext T4O = T4 * O;
  Ciphertext R = ((W * c3 - W * c5) + c1) - c7;
  Ciphertext odd = z * (R + (T4O + T4O));

  Ciphertext evenh = T4 * E;
  Ciphertext even_low = T2 * c2 - c6T2;
  Ciphertext result = odd + evenh + even_low + c0;
  result.set_output("y");
}
```