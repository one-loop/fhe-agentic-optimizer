Last attempt: the same Horner structure, but I use s = v² directly with a pre-doubled s term (c4·v + 2·c6·s). The bracket is then 2·(c4·v + 2·c6·s) + c2 − 3·c6. This removes the t4 computation and its constant subtraction, replacing them with a few cheap plaintext subtractions.

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
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext q = s * c6;
  Ciphertext p = v * c4;
  p = p + q + q;
  Ciphertext inner = p + p;
  inner = inner + c2;
  inner = inner - c6;
  inner = inner - c6;
  inner = inner - c6;
  Ciphertext r = v * inner;
  r = r + z * c1;
  r = r - c4;
  r = r + c0;
  r.set_output("y");
}
```