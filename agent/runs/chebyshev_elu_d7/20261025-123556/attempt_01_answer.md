Idea: convert to the power basis, so y = p0 + p1 z + ... + p7 z^7. The coefficients are plaintext inputs, so we can't combine them at compile time. Plaintext-plaintext arithmetic isn't allowed either, so the power-basis conversion is out. Instead we reduce the work in the Chebyshev basis.

Plan: use the doubling identities to cut depth and multiplications, with z = x*alpha costing one pt multiplication.
- T2 = 2z²-1, T3 = 2z·T2 - z, T4 = 2T2²-1 (use square), T5 = 2 T2·T3 - z, T6 = 2 T3² - 1, T7 = 2 T3·T4 - z.
- The ct-ct multiplies are z² (square), T2·T3, T3², T3·T4, T2² and z·T2, which is 6, the same as the baseline, but the depth drops.
- Depth: z is level 1, T2 level 2, T3 level 3, T4 level 3, T5 level 4, T6 level 4, T7 level 4. Then the coefficient multiplies take it to 5, against 8 before.
- Fewer levels means cheaper operations and fewer alignments.

Fewer multiplications: the odd part is z·(c1 + c3 T2' ...) but that needs plaintext arithmetic. Alternative: group terms as c1 z + c3 T3 + c5 T5 + c7 T7 and so on. This does not reduce the count, so I'll keep the depth-optimized version.

Constants: 2z²-1 is written as s+s-one, where "one" is a plaintext subtracted from a ciphertext. That is allowed because a ciphertext is involved. For T3 = 2 z T2 - z, I compute w = z*T2 and use w+w-z. Scales: z has one multiplication history and w has more, but CHEHAB aligns operands at different levels. Same-level mismatches are possible, but the baseline already did similar things.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext s = square(z);
  t[2] = s + s - one;
  Ciphertext w3 = z * t[2];
  t[3] = w3 + w3 - z;
  Ciphertext s4 = square(t[2]);
  t[4] = s4 + s4 - one;
  Ciphertext w5 = t[2] * t[3];
  t[5] = w5 + w5 - z;
  Ciphertext s6 = square(t[3]);
  t[6] = s6 + s6 - one;
  Ciphertext w7 = t[3] * t[4];
  t[7] = w7 + w7 - z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = t[1] * c1;
  result = result + t[2] * c2;
  result = result + t[3] * c3;
  result = result + t[4] * c4;
  result = result + t[5] * c5;
  result = result + t[6] * c6;
  result = result + t[7] * c7;
  result = result + c0;
  result.set_output("y");
}
```