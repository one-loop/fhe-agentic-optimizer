Idea: use R = T2·(c5 + 2·c7·T2) − c7, which avoids computing T4 and one plaintext multiply. I also precompute z·T3' in parallel and form y = 2·(z·T3')·R + z·(c3·T3' − c5 − c7 + c1) + c0. This keeps 4 ct-ct multiplies but cuts the depth to 5.

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
  Ciphertext zt3 = z * t3p;
  Ciphertext m = zt3 * R;
  Ciphertext A = m + m;
  Ciphertext Q = t3p * c3;
  Q = Q - c5;
  Q = Q - c7;
  Q = Q + c1;
  Ciphertext B = z * Q;
  Ciphertext y = A + B;
  y = y + c0;
  y.set_output("y");
}
```