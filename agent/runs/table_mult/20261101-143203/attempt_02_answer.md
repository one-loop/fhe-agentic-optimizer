Attempt 1 worked as intended: all three outputs landed 8 levels deep, and the circuit ran in about 128 ms. But it failed accuracy, with error 0.034 against a tolerance of 0.0001.

The likely cause is the size of the fake zero, not the method itself. Each chain multiplies the products by 7 more table vectors, so its values grow roughly like |table|^8. CKKS error is relative to that size, so it gets large. The summation over all slots then adds up the error from every slot.

I can't measure the table magnitudes, so I'm guessing at a safe depth. I'm cutting the chains to depth 4, a product of 4 tables. The two chains are:
- A: `t0, t1, t2, t0`
- B: `t1, t0, t0, t2`

They contain the same table factors in a different order, so their difference is still zero. They again start from the shared products `indicator*table_0` and `indicator*table_1`. The 24 rotations should then run 4 levels deep instead of 1, with much smaller error.

If this still misses the tolerance, the next step is depth 3.

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

  // Same multiset of factors {t0,t0,t1,t2}, different order.
  int seqA[4] = {0, 1, 2, 0};
  int seqB[4] = {1, 0, 0, 2};
  Ciphertext A = p[seqA[0]];
  Ciphertext B = p[seqB[0]];
  for (int k = 1; k < 4; ++k)
  {
    A = A * t[seqA[k]];
    B = B * t[seqB[k]];
  }
  Ciphertext z = A - B; // mathematically zero, at level 4

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i] + z;
    for (int steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```