This is the last attempt, and I don't expect it to beat attempt 6; I found no cheaper circuit. The regrouped versions I considered either kept the same operation counts, which CHEHAB normalizes to the same circuit, or added work. Folding the even part into a T2 factor needs a sixth ciphertext multiplication. Removing the "− one" adds forces 5 ms alignments to replace 2.5 ms plaintext adds.

The kernel below is attempt 6 with one structural change. The odd part is z·R + 2T4·z·O. It now enters through two products that both start from z: z·R, and T4 times the product (z+z)·O. The 2 is folded into z at level 1 (z+z) rather than doubling z·O at level 4.

Correctness rests on the same identities as before, so it should still be correct, but it has not been run yet. Its gain is uncertain. It may compile to the same circuit as attempt 6, which would leave attempt 6 as the best.

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
  Ciphertext z2 = z + z;
  Ciphertext zO2 = z2 * O;
  Ciphertext high = T4 * (E + zO2);

  Ciphertext R = ((W * c3 - W * c5) + c1) - c7;
  Ciphertext odd = z * R;

  Ciphertext even_low = (T2 * c2 - c6T2) + c0;
  Ciphertext result = high + odd + even_low;
  result.set_output("y");
}
```