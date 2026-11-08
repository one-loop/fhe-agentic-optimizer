Idea: use T6 = 4y³ − 3y and T4 = 2y² − 1 with y = t2. The even part becomes y·c2 − 3r + y²·(2c4 + 4r) − c4, where r = y·c6. This reuses s = y² directly, so t4 is never built and one plaintext subtraction disappears. The multiply counts stay at 3 ct-ct and 4 ct-pt.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext r = t2 * c6;
  Ciphertext r2 = r + r;
  Ciphertext u = r2 + c4;
  Ciphertext m = s * (u + u);
  Ciphertext r3 = r2 + r;
  Ciphertext result = (z * c1 + t2 * c2 - r3) + (m - c4 + c0);
  result.set_output("y");
}
```

Wait: `m - c4 + c0` uses two plaintext operations on a ciphertext, which is allowed since `m` is a ciphertext and evaluation is left to right.