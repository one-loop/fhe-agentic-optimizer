The previous failure was a same-level mismatch between c5·t5 and c6·t6. t6 had the form "2·t3² − one", while t5 had the form "2·a·b − ciphertext". This attempt makes t5, t6 and t7 all follow the same pattern, "2·a·b − (lower-depth ciphertext)":
- t5 = 2·t2·t3 − z
- t6 = 2·t2·t4 − t2
- t7 = 2·t3·t4 − z

The circuit is still 6 ct-ct multiplies at depth 4. The accumulation order stays deep-to-shallow.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext a6 = t2 * t4;
  Ciphertext t6 = a6 + a6 - t2;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + t4 * c4;
  result = result + t3 * c3;
  result = result + t2 * c2;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
```