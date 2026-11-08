Idea: compute the c2 term from u instead of t2, using c2·t2 = 2·(c2·u) − c2. The plaintext multiply is the same count, but it no longer waits on the `one` subtraction that builds t2. The cost is one extra plaintext subtraction of c2, so this is a small gamble.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t2 * c6;
  Ciphertext v = w + w + c4;
  Ciphertext m = t4 * v;
  Ciphertext uc = u * c2;
  Ciphertext result = z * c1;
  result = result + uc + uc;
  result = result - c2;
  result = result - w;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}
```