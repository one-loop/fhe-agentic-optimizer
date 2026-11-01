The baseline's cost is almost entirely its 24 rotations, all at level 1. They are needed: without mask plaintexts the three sums cannot share a reduction. So instead of removing rotations, I try to make each one cheaper by running it at a much deeper level, where key switching works on fewer primes.

To get there, I build a ciphertext that equals zero mathematically but sits at depth 8. It is the difference of two ct×pt product chains that use the same tables in different orders. The two chains start from `indicator*table_0` and `indicator*table_1`, which the outputs need anyway, so those products are shared. Adding this zero to each product forces CHEHAB to align it down to the deep level, so all 24 rotations run cheaply. The extra cost is about 14 ct×pt multiplies, which are cheap at deep levels.

Two things could go wrong:
- **Simplification:** CHEHAB's rewrites might recognise the zero and cancel it, which would leave the circuit unchanged.
- **Precision:** the leftover CKKS error in the zero grows with the size of the table values.

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

  // Two chains with the same multiset of table factors in different order:
  // A: t0,t1,t2,t0,t1,t2,t0,t1   B: t1,t0,t2,t1,t0,t2,t1,t0
  int seqA[8] = {0, 1, 2, 0, 1, 2, 0, 1};
  int seqB[8] = {1, 0, 2, 1, 0, 2, 1, 0};
  Ciphertext A = p[seqA[0]];
  Ciphertext B = p[seqB[0]];
  for (int k = 1; k < 8; ++k)
  {
    A = A * t[seqA[k]];
    B = B * t[seqB[k]];
  }
  Ciphertext z = A - B; // mathematically zero, at deep level

  for (int i = 0; i < 3; ++i)
  {
    Ciphertext r = p[i] + z;
    for (int steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```