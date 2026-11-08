**Change:** I tried to cut down CHEHAB's own scale-alignment work, which is the remaining overhead. In attempt 4 the level-2 sum s2 = (c1 − c7)·T1 was first aligned to level 4 and then dragged to level 5 with the rest of s. Now only the level-3 and level-4 corrections (s3, s4 − a2) are combined at level 4. The level-2 piece s2 goes straight into the final level-5 sum, so it is aligned once, directly to level 5. The mod_switch work in between should mostly disappear.

The rest is the same as attempt 4: T4 + 1 is used as the giant step, the −a2 correction is merged at level 4, and the circuit keeps 4 ct-ct and 9 ct-pt multiplications at depth 5. The expected gain is small and may not clear the 3% bar.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;                 // level 1
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;             // level 2
  Ciphertext u = t2 + t2 - one;              // 2*T2 - 1, level 2
  Ciphertext t3 = t1 * u;                    // level 3
  Ciphertext t22 = t2 * t2;
  Ciphertext t44 = t22 + t22;                // T4 + 1, level 3

  // A = c5 T1 + c6 T2 + c7 T3
  Ciphertext c6t2 = t2 * c6;
  Ciphertext a = t1 * c5 + (c6t2 + t3 * c7);
  Ciphertext a2 = a + a + c4;                // level 4
  Ciphertext w = t44 * a2;                   // level 5

  Ciphertext s2 = t1 * c1 - t1 * c7;         // level 2
  Ciphertext s3 = t2 * c2 - c6t2;            // level 3
  Ciphertext s4 = t3 * c3 - t3 * c5;         // level 4
  Ciphertext s34 = s3 + (s4 - a2);           // level 4

  Ciphertext result = (w + s34) + (s2 + c0);
  result.set_output("y");
}
```