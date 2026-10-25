Idea: since t4 = 2·u2 − 1, I use u2 directly. Then c4·T4 + 2·c6·T2·T4 becomes 2·(u2·m) − m, where m = c4 + 2·c6·T2. This drops the `u2+u2−one` step, including an encoded plaintext subtraction. The ciphertext and plaintext multiplication counts stay at 3 and 4, but the circuit has fewer additions and subtractions and one less plaintext encoding. The product u2·m sits at the same level as before (level 4).

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext a6 = t2 * c6;
  Ciphertext m = a6 + a6 + c4;
  Ciphertext pm = u2 * m;
  Ciphertext low = t2 * c2 + z * c1;
  low = low - a6 - m;
  Ciphertext result = pm + pm + low;
  result = result + c0;
  result.set_output("y");
}
```