The scale mismatch came from `q` and `c4*s` both landing at level 4 with different multiplication histories. I fold the c4 term into the Horner step instead: q = s·(4·c6·t2 + 2·c4). The plaintext c4 is added to a ciphertext, which is allowed. This drops one plaintext multiplication and leaves a single level-4 term, so nothing mismatches.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext w = t2 * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4;
  inner = inner + c4;
  Ciphertext q = inner * s;
  Ciphertext b = t2 * c2;
  Ciphertext w3 = w2 + w;
  Ciphertext result = q + b;
  result = result - w3;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```