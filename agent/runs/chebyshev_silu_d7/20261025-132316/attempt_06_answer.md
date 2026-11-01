Idea: do the plaintext adds at the cheapest place. I fold c0 − c4 into the low-level c1·z term, which sits at level 2, and add c2 − c6 to the shallower c4·v product before it meets the T4 term. This avoids extra scale alignments at the end.

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
  Ciphertext low = z * c1;
  low = low + c0;
  low = low - c4;
  Ciphertext u = square(z);
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext t4 = s + s - one;
  Ciphertext a = (v + v) * c4;
  a = a + c2;
  a = a - c6;
  Ciphertext b = t4 * c6;
  Ciphertext inner = a + b + b;
  Ciphertext r = v * inner;
  r = r + low;
  r.set_output("y");
}
```