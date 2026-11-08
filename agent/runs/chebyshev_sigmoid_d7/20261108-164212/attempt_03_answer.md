The last failure was the low terms (c1·z and c3·T3) landing at level 4 with a different scale from the c5 term. I now compute the c3 term as (c3·z)·(2·T2 − 1) so it ends at level 3, below the c5 term at level 4 and the c7 term at level 5.

T3 is now z·(2·T2 − 1), which replaces z·w. This costs one extra ciphertext-ciphertext multiplication, so the count is 6, the same as the baseline. The depth should still drop from 8 to about 5. I have not compiled or timed this version.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext p5 = (t2 * c5) * t3;
  Ciphertext term5 = p5 + p5 - z * c5;
  Ciphertext p7 = (t4 * c7) * t3;
  Ciphertext term7 = p7 + p7 - z * c7;
  Ciphertext low = z * c1 + (z * c3) * u;
  Ciphertext result = low + term5;
  result = result + term7;
  result = result + c0;
  result.set_output("y");
}
```