Both remaining ideas would save at most a few percent, and a single timing may not confirm a gain.

**Why the structure is at its floor.** It uses 3 ct-ct multiplications, 3 plaintext multiplications (`alpha`, `c1`, `c6`) and depth 5. Working through the possible depth-4 forms, each one needs a fourth plaintext multiplication to bring in `c2`, so this looks like the minimum for this approach.

**What this attempt changes.** It combines attempt 5's form (v = 2u at level 2, mid = v·h + c2) with attempt 3's handling of −3c6:
- The −3c6 constant is added as plaintext additions inside `mid`, before the last multiplication by u.
- That replaces the separate level-3 `s` correction term. Only the odd term `c1·z` then has to be aligned to the deep product.

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
  Ciphertext w = z * z;
  Ciphertext u = w + w - one;          // T2
  Ciphertext v = u + u;                // 2u
  Ciphertext s = u * c6;               // c6 u
  Ciphertext h = s + s + c4;           // 2 c6 u + c4
  Ciphertext mid = v * h + c2 - c6 - c6 - c6;  // 4c6u^2 + 2c4u + c2 - 3c6
  Ciphertext e = u * mid;              // 4c6u^3 + 2c4u^2 + (c2-3c6)u
  Ciphertext lin = z * c1;
  Ciphertext result = e + lin + c0 - c4;
  result.set_output("y");
}
```