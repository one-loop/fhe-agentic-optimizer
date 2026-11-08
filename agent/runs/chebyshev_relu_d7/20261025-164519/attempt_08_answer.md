Last attempt: use the attempt 3 form, where −3c6 is folded in as plaintext subtractions, so the ciphertext w is no longer needed. Instead I build 4s with cheap ciphertext adds at s's level and multiply once by c6 to get inner = 4c6·s + 2c4. This keeps one pt multiply and puts the doubling adds on a higher-level, cheaper-to-align ciphertext.

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
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext s = u2 - one;
  Ciphertext u4 = u2 + u2;
  Ciphertext s4 = u4 + u4;
  Ciphertext g = s4 * c6;
  Ciphertext h = g + c4;
  Ciphertext inner = h + c4;
  Ciphertext m = s * inner;
  m = m + c2;
  m = m - c6;
  m = m - c6;
  m = m - c6;
  Ciphertext r = s * m;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```

Wait: s4 here is 4u, not 4s, which is wrong. A correct version needs 4s = 4(2u − 1), which needs plaintext arithmetic. I must therefore build it from s itself:

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
  Ciphertext u = square(z);
  Ciphertext s = u + u - one;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext g = s4 * c6;
  Ciphertext h = g + c4;
  Ciphertext inner = h + c4;
  Ciphertext m = s * inner;
  m = m + c2;
  m = m - c6;
  m = m - c6;
  m = m - c6;
  Ciphertext r = s * m;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```

This second block is the kernel I am submitting; the first one is incorrect and should be ignored.