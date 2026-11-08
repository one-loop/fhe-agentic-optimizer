Same-level mismatches only arise when two terms meet at the same level with different multiplication histories. The three deepest terms (c5·t5, c6·t6, c7·t7) all land at level 5. I shift c6·t6 down one level and c7·t7 down two by multiplying with the `one` plaintext. That costs about 5 ms per extra plaintext multiply. All three terms then sit at distinct levels (5, 6, 7), and every addition pairs the deeper accumulator with a shallower term, so alignment is free. The rest keeps the low-depth doubling scheme.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext u7 = (t7 * c7) * one;
  u7 = u7 * one;
  Ciphertext u6 = (t6 * c6) * one;
  Ciphertext u5 = t5 * c5;
  Ciphertext result = u7 + u6;
  result = result + u5;
  result = result + t4 * c4;
  result = result + t3 * c3;
  result = result + t2 * c2;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
```