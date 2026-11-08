The previous attempt failed because adding the plaintext c1 before the multiplication gave `low` a different scale history from `high * u2` at the same level. Here I drop the plaintext add: `low = t*(u*c3) + t*c1`. The deep term now has the same kind of multiplication history as the high part, and `t*c1` sits at a shallower level, so CHEHAB can align it for free. I haven't confirmed this resolves the mismatch.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
  {
    string p = "s" + to_string(stage) + "c";
    Plaintext c1(p + "1"), c3(p + "3"), c5(p + "5"), c7(p + "7");
    Ciphertext u = t * t;
    Ciphertext u2 = u * u;
    Ciphertext low = t * (u * c3) + t * c1;
    Ciphertext high = (t * c5) + (t * c7) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```