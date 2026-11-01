I don't see a variant that changes the circuit and is likely to be faster, so I'm returning the current best kernel (attempt 5) unchanged. It will not win a new ranking. I think it is the best this approach can do here:

- **Why the speedup works.** The 24 rotations are unavoidable without mask plaintexts. The gain comes from pushing most of them to level 2 with a fake zero, `z = (indicator*t0)*t1 − (indicator*t1)*t0`.
- **Deeper fake zeros fail on accuracy.** Each extra level multiplies the error in `z` by roughly |table|, which looks to be about 100. Attempts 1 and 2 failed this way.
- **Adding `z` earlier is too close to the limit.** Adding it before any reduction step gave an error of 1.00006e-4 against a 1e-4 tolerance (attempt 3). Attempt 6's one-step version passed at 5.4e-5 but compiled to the same circuit, so it couldn't be ranked.
- **`table_2` can't be used.** It appears to be all zeros in the fixed test case, which crashes with a transparent-ciphertext error (attempt 4).
- **The 3 scale alignments can't be shared.** Each output's partial sum has to be aligned separately to `z`'s level.
- **Pushing `indicator` itself deeper doesn't work.** The noise in `z` would then be multiplied by a table value, which breaks the tolerance.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  vector<Plaintext> t;
  for (int i = 0; i < 3; ++i)
    t.emplace_back("table_" + to_string(i));

  vector<Ciphertext> p;
  for (int i = 0; i < 3; ++i)
    p.push_back(indicator * t[i]);

  Ciphertext A = p[0] * t[1];
  Ciphertext B = p[1] * t[0];
  Ciphertext z = A - B; // mathematically zero, at level 2

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i];
    int steps = slots() >> 1;
    for (int k = 0; k < 2 && steps > 0; ++k, steps >>= 1)
      r += r << steps;
    r = r + z;
    for (; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```