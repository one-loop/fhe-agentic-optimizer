Idea: restructure so the plaintext coefficients are applied late and only once each. With w = z², u = w − 1 and p = w·u, the polynomial becomes y = z·S, where
S = c3·(w + 3u) + c5·(16p − 4u) + c7·(64w·p − 48p + 8u) + (c1 + c5 + c7).
The three constants are added as plaintexts. This needs 4 ciphertext multiplications (w, p, w·p, and the final z·S) and 4 plaintext multiplications (alpha, c3, c5, c7). The plaintext multiplications by c_k now happen at deeper, cheaper levels.

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
  Ciphertext z = x * alpha;
  Ciphertext w = square(z);
  Ciphertext u = w - one;
  Ciphertext p = w * u;
  Ciphertext wp = w * p;
  auto dbl = [](const Ciphertext &a) { return a + a; };
  Ciphertext u2 = dbl(u);
  Ciphertext u4 = dbl(u2);
  Ciphertext u8 = dbl(u4);
  Ciphertext u3 = u2 + u;
  Ciphertext A3 = w + u3;
  Ciphertext p2 = dbl(p);
  Ciphertext p4 = dbl(p2);
  Ciphertext p16 = dbl(dbl(p4));
  Ciphertext p32 = dbl(p16);
  Ciphertext p48 = p32 + p16;
  Ciphertext p64 = dbl(p32);
  Ciphertext A5 = p16 - u4;
  Ciphertext wp64 = dbl(dbl(dbl(dbl(dbl(dbl(wp))))));
  Ciphertext A7 = (wp64 - p48) + u8;
  Ciphertext S = A3 * c3;
  S = S + A5 * c5;
  S = S + A7 * c7;
  S = S + c1;
  S = S + c5;
  S = S + c7;
  (void)p64;
  Ciphertext y = z * S;
  y.set_output("y");
}
```