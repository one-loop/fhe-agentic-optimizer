Attempt 3 cut plaintext additions but added two scale alignments. They came from subtracting `n` (level 4) and `g` (level 3) from the level-5 product. This version is a middle ground between attempts 1 and 3:

- It restores q = 2T2 − 1 and computes q·n directly, which removes the `n` subtraction.
- It keeps the attempt-3 recurrence, so only 7 plaintext additions remain, against 9 in attempt 1.
- Still 4 ciphertext-ciphertext and 2 ciphertext-plaintext multiplications, 6 levels.

The `g` subtraction from the level-5 product remains, so expect about one scale alignment. Any gain over attempt 1 would be a few milliseconds and may not clear the 3% bar.

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
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zs = z * z;
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7 T2
  Ciphertext g = u + u + c5;              // c5 + 2 c7 T2
  Ciphertext h = t2 * g;
  Ciphertext n = h + h + c3 - c7;         // c3 - c7 + 2 T2 g
  Ciphertext q = t2 + t2 - one;           // 2T2 - 1
  Ciphertext qn = q * n;
  Ciphertext s = qn - g + c1;             // c1 - g + q n
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```