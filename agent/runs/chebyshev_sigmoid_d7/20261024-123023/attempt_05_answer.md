Attempt 4 had 5 ct-ct multiplies, because z·Q is itself a ct-ct multiply, which cost more than the depth saved. So I return to attempt 3's structure with 4 multiplies (z², T2·w, T3'·R, z·S) and use the cheaper R = T2·(c5 + 2·c7·T2) − c7, which saves one plaintext multiply and the T4 term.

```cpp
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
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext v = u - one;
  Ciphertext v2 = v + v;
  Ciphertext v4 = v2 + v2;
  Ciphertext t3p = v4 + one;
  Ciphertext c7t2 = t2 * c7;
  Ciphertext w = c7t2 + c7t2;
  w = w + c5;
  Ciphertext R = t2 * w;
  R = R - c7;
  Ciphertext m = t3p * R;
  Ciphertext S = m + m;
  S = S - c5;
  S = S - c7;
  S = S + t3p * c3;
  S = S + c1;
  Ciphertext y = z * S;
  y = y + c0;
  y.set_output("y");
}
```