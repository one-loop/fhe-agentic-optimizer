The earlier rewrite collapsed to the baseline circuit, so this attempt changes the structure to cut ciphertext-plaintext work. For each stage, with u = t², I compute the low part as t·(c1 + c3·u), which needs only one scalar multiplication plus a cheap plaintext add. The high part is (c5·t + (c7·t)·u)·u². Each stage still uses 5 ct-ct multiplications and stays at depth 3, but it needs 3 ct-pt multiplications instead of 4. I haven't confirmed the circuit comes out different from the baseline.

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
    Ciphertext low = t * ((u * c3) + c1);
    Ciphertext high = (t * c5) + (t * c7) * u;
    t = low + high * u2;
  }
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```