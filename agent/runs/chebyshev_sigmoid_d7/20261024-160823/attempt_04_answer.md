This attempt reduces ct-ct multiplications to 4 by using the identity T_k/z for odd k:
- T3/z = 2·T2 − 1
- T5/z = 2·T4 − 2·T2 + 1
- T7/z = 2·T6 − 2·T4 + 2·T2 − 1
- T6 = 2·T2·T4 − T2

So y = c0 + z·(c1 + c3·g3 + c5·g5 + c7·g7), with g3, g5, g7 built from T2, T4 and T2·T4. The ct-ct multiplications are z², T2², T2·T4 and the final z·Q. The three coefficient terms land at distinct levels (3, 4 and 5), which should avoid scale mismatches.

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext m = t2 * t4;
  Ciphertext d = t4 - t2;
  Ciphertext e = m + m - t4;
  Ciphertext g3 = t2 + t2 - one;
  Ciphertext g5 = d + d + one;
  Ciphertext g7 = e + e - one;
  Ciphertext q = g3 * c3;
  q = q + g5 * c5;
  q = q + g7 * c7;
  q = q + c1;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
```