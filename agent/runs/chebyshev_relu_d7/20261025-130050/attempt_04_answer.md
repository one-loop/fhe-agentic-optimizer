The previous attempt failed because c4·T4 and (c6·t)·h met at the same level with different multiplication histories. This version removes that sum by factoring s out of the whole even part:

y = s·(4·c6·t + 2·c4) + (c2·t − 3·c6·t) + c1·z + c0 − c4

- The ct-ct product s·g is the only deep term, at level 4. g = 4·(c6·t) + c4 + c4 is formed at level 3.
- Every other term is at level 3 or lower and uses the same pt-multiplication pattern.
- Operation counts: 3 ct-ct multiplications and 4 plaintext multiplications (alpha, c6·t, c2·t, c1·z), at depth 4.

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
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext tc6 = t * c6;
  Ciphertext tc6_2 = tc6 + tc6;
  Ciphertext g = tc6_2 + tc6_2 + c4 + c4;
  Ciphertext low = t * c2 - (tc6_2 + tc6) + z * c1 + c0 - c4;
  Ciphertext result = s * g + low;
  result.set_output("y");
}
```