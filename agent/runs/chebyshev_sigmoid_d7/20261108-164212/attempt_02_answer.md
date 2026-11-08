The first attempt failed because the c5 and c7 terms met at level 5 with different scale histories. I fix this by pushing the c5 and c7 multiplications earlier, so each term ends at a different level:

- The c7 term is 2·(c7·T4)·T3 − c7·z, which ends at level 5.
- The c5 term is 2·(c5·T2)·T3 − c5·z, which ends at level 4.
- The c3 term and the c1 term end at lower levels, so CHEHAB aligns them for free.

The c3·T3 term may still land at level 4 with a different scale history from the c5 term. To avoid that, I add c1·z to c3·T3 first and then add the c5 term, and I haven't compiled this version yet.

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
  Ciphertext zw = z * w;
  Ciphertext zw2 = zw + zw;
  Ciphertext t3 = zw2 + zw2 - z - z - z;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext p5 = (t2 * c5) * t3;
  Ciphertext term5 = p5 + p5 - z * c5;
  Ciphertext p7 = (t4 * c7) * t3;
  Ciphertext term7 = p7 + p7 - z * c7;
  Ciphertext low = z * c1 + t3 * c3;
  Ciphertext result = low + term5;
  result = result + term7;
  result = result + c0;
  result.set_output("y");
}
```