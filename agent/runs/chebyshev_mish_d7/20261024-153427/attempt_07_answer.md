I don't have a confident improvement left. This attempt is the even/odd split again, with the doubling moved from a precomputed w = t + t to per-step adds, the way attempt 3 did it. That changes the operation mix, but it may not beat attempt 3.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t = z2 + z2 - one;

  // even part
  Ciphertext pe = t * c6;
  Ciphertext e2 = pe + pe + c4;
  Ciphertext m1 = t * e2;
  Ciphertext e1 = m1 + m1 + c2 - c6;
  Ciphertext ev = t * e1 + c0 - e2;

  // odd part
  Ciphertext po = t * c7;
  Ciphertext d2 = po + po + c5;
  Ciphertext n1 = t * d2;
  Ciphertext d1 = n1 + n1 + c3 - c7;
  Ciphertext n0 = t * d1;
  Ciphertext d0 = n0 + n0 + c1 - d2;
  Ciphertext od = z * (d0 - d1);

  Ciphertext y = ev + od;
  y.set_output("y");
}
```