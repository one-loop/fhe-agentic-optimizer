Four levels and these operation counts look like a floor for this basis, so this attempt only moves where things are added.

- **c0 at the end:** the plaintext add of c0 moves to the final level-4 ciphertext. That ciphertext carries fewer primes, so encoding and adding should be cheaper.
- **c1·z at the end:** the linear term c1·z is also added at the end. CHEHAB can then align it directly to the output level, instead of first lifting it to level 3 and then aligning the level-3 sum again.
- **Unchanged:** the rest keeps the attempt-3 form T4·w = 2(u²·w) − w, which avoids one plaintext add.

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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext zz = z * z;               // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext uu = u * u;               // level 3
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext p = uu * w;               // level 4
  Ciphertext mid = u * c2 - c6u - w;   // level 3
  Ciphertext lin = z * c1;             // level 2
  Ciphertext high = p + p + mid;       // level 4
  Ciphertext result = (high + lin) + c0;
  result.set_output("y");
}
```