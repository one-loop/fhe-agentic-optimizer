Every variant of attempt 4 so far has come out within timing noise, so I'm going back to the depth-4 structure of attempt 1 and computing z² with `square(z)`. Attempt 1 uses one fewer level than attempt 4, which makes every operation cheaper. Its drawback is two extra scalar plaintext multiplications. The cheaper square should narrow that gap, though it may not be enough to win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext z2 = square(z);           // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext q3 = u + u - one;         // 2u - 1
  Ciphertext zq3 = z * q3;             // T3, level 3
  Ciphertext c5u = u * c5;             // level 3
  Ciphertext r = c5u + c5u + c3;       // c3 + 2 c5 u
  Ciphertext big = zq3 * r;            // level 4
  Ciphertext lin = z * c1 - z * c5;    // level 2
  Ciphertext result = big + lin + c0;
  result.set_output("y");
}
```