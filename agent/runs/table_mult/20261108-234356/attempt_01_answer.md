I don't expect a kernel meaningfully faster than the baseline to exist. The kernel below is an equivalent rewrite, and it will most likely not be accepted as a new best.

**Why the baseline is already minimal:**
- **Multiplications.** Each output depends on the slot-wise product `indicator * table_i`, so the 3 plaintext multiplications and their 3 rescales are needed. That also fixes the depth at 1 level.
- **No masks.** The CKKS rules forbid numeric constants and plaintext-plaintext operations. So no masking plaintext can be built to pack the three products into one ciphertext and reduce them together. The tables can't serve as masks because their values are arbitrary. The same rules rule out pre-rotating the tables, which would let the indicator's rotations be shared across columns.
- **Rotations can't be shared.** Each output is a combination of rotations and additions of the three products. Products belonging to other outputs must cancel exactly for arbitrary data. Each output therefore needs its own all-ones (sum-over-slots) operator, which takes log₂(256) = 8 rotations. The three outputs are independent, so the bound is 3·8 = 24 rotations, which the baseline already uses.

**What the kernel changes:** it folds with step sizes 1, 2, 4, …, 128 instead of 128 down to 1, and writes each product as its own named intermediate. The operation counts and depth are the same as the baseline. If CHEHAB normalises both versions to the same circuit, it can't win under the judging rules.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext t0("table_0");
  Plaintext t1("table_1");
  Plaintext t2("table_2");
  vector<Ciphertext> prods;
  prods.push_back(indicator * t0);
  prods.push_back(indicator * t1);
  prods.push_back(indicator * t2);
  int n = slots();
  for (size_t i = 0; i < prods.size(); ++i)
  {
    Ciphertext r = prods[i];
    for (int steps = 1; steps < n; steps <<= 1)
      r += r << steps;
    r.set_output("output_" + to_string(i));
  }
}
```