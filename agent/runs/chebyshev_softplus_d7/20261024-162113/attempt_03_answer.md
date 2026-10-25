Idea: lower the depth. I multiply c6 into T2 early, which is cheap because T2 sits at a shallow level, and then multiply by w = 2·T4 − 1. The last ciphertext product then happens one level earlier. This costs one extra plaintext multiply (c2·T2), but it trades c6·w + c2 for work at shallower levels and cuts the depth to 4.

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
  Ciphertext t4s = square(t2);
  Ciphertext t4 = t4s + t4s - one;
  Ciphertext w = t4 + t4 - one;
  Ciphertext a = t2 * c6;
  Ciphertext result = z * c1;
  result = result + t4 * c4;
  result = result + t2 * c2;
  result = result + a * w;
  result = result + c0;
  result.set_output("y");
}
```