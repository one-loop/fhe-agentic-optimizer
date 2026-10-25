Idea: keep the attempt-5/6 structure, but do the ×64 scaling on w before the multiplication (w64·p instead of 64·(w·p)). The doublings then run on a shallower, cheaper ciphertext, and the sum S is formed in a different order. The multiplication count is unchanged, so this is a small tweak.

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
  auto dbl = [](const Ciphertext &a) { return a + a; };
  Ciphertext w64 = dbl(dbl(dbl(dbl(dbl(dbl(w))))));
  Ciphertext wp64 = w64 * p;
  Ciphertext u2 = dbl(u);
  Ciphertext u4 = dbl(u2);
  Ciphertext u8 = dbl(u4);
  Ciphertext u3 = u2 + u;
  Ciphertext A3 = w + u3;
  Ciphertext p16 = dbl(dbl(dbl(dbl(p))));
  Ciphertext p48 = p16 + dbl(p16);
  Ciphertext A5 = p16 - u4;
  Ciphertext A7 = (wp64 - p48) + u8;
  Ciphertext S = A3 * c3;
  S = S + c1;
  S = S + c5;
  S = S + c7;
  S = S + A5 * c5;
  S = S + A7 * c7;
  Ciphertext y = z * S;
  y.set_output("y");
}
```