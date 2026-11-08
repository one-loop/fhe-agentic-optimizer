Idea: the Clenshaw recurrence was a big win (251 ms against 380 ms). I now run it in the variable y = T2(z), splitting the polynomial into odd and even parts. The multiplicative depth drops from 8 to about 6 while the ct-ct count stays at 6, and the cheaper deeper levels help.

- Odd part: z·(c1 − b5 + b3·(W−1)), with b7 = c7, b5 = c5 + W·c7, b3 = c3 + W·b5 − c7 and W = 2·T2.
- Even part: Clenshaw in y = T2 with coefficients c0, c2, c4, c6.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext y = z2 + z2 - one;
  Ciphertext W = y + y;

  // odd part
  Ciphertext b5 = W * c7 + c5;
  Ciphertext b3 = (W * b5 + c3) - c7;
  Ciphertext inner = (W * b3 - b3 - b5) + c1;
  Ciphertext S = z * inner;

  // even part
  Ciphertext f2 = W * c6 + c4;
  Ciphertext f1 = (W * f2 + c2) - c6;
  Ciphertext E = (y * f1 + c0) - f2;

  Ciphertext r = S + E;
  r.set_output("y");
}
```