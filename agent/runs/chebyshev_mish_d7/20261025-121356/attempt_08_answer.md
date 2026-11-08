This is attempt 7 with the `c0` add moved onto the shallow `z*c1` term, as in attempt 6. It is a distinct circuit, and at that level the add is cheaper.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext b = t2 + t2 - one;
  Ciphertext t3 = z * b;
  Ciphertext t2s = square(t2);
  Ciphertext D0 = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext D = D0 + D0 + c4;
  Ciphertext zc1 = z * c1 + c0;
  Ciphertext low = zc1 + t2 * c2 + t3 * c3;
  Ciphertext sub = t3 * c5 + t2 * c6 + z * c7;
  Ciphertext m = t2s * D;
  Ciphertext y = (low - sub) - D + m + m;
  y.set_output("y");
}
```