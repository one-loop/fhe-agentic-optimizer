Idea: with T3' = T3/z = 4u−3, T5' = 2·T2·T3'−1 and T7' = 2·T4·T3'−1, the odd polynomial becomes y = c0 + z·S, where S = c1 + c3·T3' + 2·T3'·(c5·T2 + c7·T4) − c5 − c7. This needs only 4 ct-ct multiplies (z², T2², T3'·R, z·S). Everything else is cheap plaintext multiplies and adds, and every sum combines terms at different levels.

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
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext R = t2 * c5 + t4 * c7;
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