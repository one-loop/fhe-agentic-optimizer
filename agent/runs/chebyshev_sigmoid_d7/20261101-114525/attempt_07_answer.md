Attempt 6 showed that `square(z)` is marginally cheaper than `z*z`, but not by the 3% needed. This attempt combines that change with attempt 4's form, which has the fewest plaintext additions. That form uses T7 = 2·T2·T5 − T3 and T5 = 2·T2·T3 − T1, giving y = c0 + z·[c1 − g + (2T2 − 1)·n] with g = c5 + 2c7·T2 and n = 2·T2·g + c3 − c7.

It keeps 4 ciphertext multiplications (one of them a square), 2 scalar multiplications, 6 levels and 7 plaintext additions. The g subtraction still needs one scale alignment, so a confirmed win is unlikely.

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
  Ciphertext zs = square(z);
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7 T2
  Ciphertext g = u + u + c5;              // c5 + 2 c7 T2
  Ciphertext h = t2 * g;
  Ciphertext n = h + h + c3 - c7;         // c3 - c7 + 2 T2 g
  Ciphertext q = (t2 - one) + t2;         // 2T2 - 1
  Ciphertext qn = q * n;
  Ciphertext s = qn - g + c1;             // c1 - g + q n
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```