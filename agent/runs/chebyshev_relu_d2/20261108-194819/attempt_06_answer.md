No new structure here: I believe attempt 1 is the minimum circuit for this function, and the operation counts back that up.

- **Levels:** the term 2·α²·c2·x² multiplies five factors (x, x, α, α, c2). A binary multiplication tree with five leaves needs depth 3, so 3 levels is the floor.
- **Multiplications:** attempt 1 already has 1 ciphertext×ciphertext multiply, 2 ciphertext×plaintext multiplies, 3 rescales, and no scale matching.
- **Additions:** the 4 additions are irreducible, because plaintext-plaintext arithmetic is not allowed.
- **Remaining overhead:** only one ~1 ms mod switch, which aligns z (level 1) with v (level 2). Removing it costs at least one more plaintext multiply (~5 ms).

Every alternative I checked is slower:
- Pairing two level-1 branches as (x·α)·(x·c2), then fixing up α and c1, needs 4 plaintext multiplies.
- Starting from `square(z)` needs 3 plaintext multiplies plus a scale match.
- Putting the ciphertext×ciphertext multiply on raw x (attempt 3) adds a level and measured 26 ms.

The kernel below is attempt 1 unchanged, so it will not be timed again. I recommend keeping it as the result: 17.0 ms, against 24.0 ms for the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext v = w + w + c1;
  Ciphertext p = z * v;
  Ciphertext y = p - c2 + c0;
  y.set_output("y");
}
```