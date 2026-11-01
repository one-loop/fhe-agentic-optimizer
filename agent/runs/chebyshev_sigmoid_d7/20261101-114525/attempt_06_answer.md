Attempt 1 at 100.78 ms is very likely the floor for this function, and attempts 3 to 5 confirm it. The kernel below is the same mathematical form, so it is unlikely to beat it by the required 3%.

- **Multiplications.** The odd degree-7 polynomial is z·B(T2) with B cubic. Getting T2 costs one ciphertext multiplication, the final multiplication by z costs another, and B needs two more. The coefficients can't be combined in plaintext, so B can't be factored to save one. That makes 4 ciphertext multiplications.
- **Scalar multiplications.** Both are required: `alpha` for z, and one coefficient (c7) to start the nested evaluation.
- **Fewer levels.** The only 5-level form I found (attempt 2) needs extra scalar multiplications, and it measured slower.
- **Plaintext additions.** Removing some (attempts 3 to 5) introduced scale alignments that cancelled the savings.

The only change from attempt 1 is computing T2 from `square(z)` and building 2T2 − 1 as (T2 − one) + T2. The second regrouping might, at best, slightly change how CHEHAB places the alignments.

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
  Ciphertext zs = square(z);
  Ciphertext t2 = zs + zs - one;          // T2
  Ciphertext u = t2 * c7;                 // c7*T2
  Ciphertext v = u + u + c5;              // 2c7 T2 + c5
  Ciphertext w = t2 * v;                  // 2c7 T2^2 + c5 T2
  Ciphertext inner = w - c7;              // c7 T4 + c5 T2
  Ciphertext m = inner + inner + c3;      // c3 + 2 inner
  Ciphertext q = (t2 - one) + t2;         // 2T2 - 1
  Ciphertext qm = q * m;
  Ciphertext s = qm + c1 - c5 - c7;
  Ciphertext r = z * s;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```