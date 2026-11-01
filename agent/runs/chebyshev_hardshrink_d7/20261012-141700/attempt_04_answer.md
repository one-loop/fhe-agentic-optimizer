**Idea:** I could not find a structural change that beats the attempt-2 kernel, so I am resubmitting it as the best result.

The four ciphertext-ciphertext multiplies (T2, T3, T5, T7) dominate the cost, at about 30 ms each. I checked whether a degree-7 polynomial can be built with three:

- Any product of two ciphertexts with degrees summing to 7 needs a factor of degree at least 4. Reaching degree 4 takes two multiplies (T2², or z·T3 with T3 = z·T2), so the chain is at least four.
- Re-associating with plaintext linear combinations does not help. Writing c7·T7 = (2c7·T2)·T5 − c7·T3 and merging the c5 term gives a different grouping, but still four ct-ct products and about the same number of ct-pt products.

The ct-pt work (about 30 ms across five multiplies) and the level alignments are already small. Changing the summation order, as in attempts 2 and 3, moved the time by less than 1 ms, which is within run-to-run noise. The kernel below is correct to about 1e-7 and runs in about 136 ms, down from the 161 ms baseline. I don't see a further gain available within these CKKS rules.

```cpp
// Chebyshev hardshrink degree 7 on [-4,4].
// Odd terms only: T2 = 2z^2-1, T3 = 2z*T2 - z, T5 = 2*T2*T3 - z, T7 = 2*T2*T5 - T3.
// Four ciphertext-ciphertext multiplies, depth 6. Sum ordered deepest-first.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;

  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;

  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;

  Ciphertext T2T3 = T2 * T3;
  Ciphertext T5 = T2T3 + T2T3 - z;

  Ciphertext T2T5 = T2 * T5;
  Ciphertext T7 = T2T5 + T2T5 - T3;

  Ciphertext t7 = T7 * c7;
  Ciphertext t5 = T5 * c5;
  Ciphertext t3 = T3 * c3;
  Ciphertext t1 = z * c1;

  Ciphertext result = t7 + t5;
  result = result + t3;
  result = result + t1;
  result = result + c0;

  result.set_output("y");
}
```