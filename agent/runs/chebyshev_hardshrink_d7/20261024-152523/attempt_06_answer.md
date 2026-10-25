Idea: keep the attempt-5 structure, which is slightly faster, but add the lowest-level terms first. The plaintext constants go onto the shallow c3 term at level 3, so the sum climbs level by level, from c3·A3 to c5·A5 to c7·A7. This should cut the scale alignments (match_scale) needed when terms of different depth meet.

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
  Ciphertext A5 = p16 - u4;
  Ciphertext wp64 = dbl(dbl(dbl(dbl(dbl(dbl(wp))))));
  Ciphertext A7 = (wp64 - p48) + u8;
  Ciphertext T = A3 * c3;
  T = T + c1;
  T = T + c5;
  T = T + c7;
  Ciphertext T5 = A5 * c5;
  Ciphertext T7 = A7 * c7;
  Ciphertext S = (T + T5) + T7;
  Ciphertext y = z * S;
  y.set_output("y");
}
```