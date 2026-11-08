Idea: the baseline performs three full rotate-and-add reductions (3 × 14 = 42 rotations before CSE, 26 after). Instead, I multiply the indicator by each table column and mask each product into its own slot before reducing, so one reduction suffices.

Let t_i = indicator * table_i. Each coordinate is the sum over all slots of t_i, which a reduction places in every slot. Because the reduction is linear, I can combine the columns first. Define

 C = t_0 * first_slot + (t_1 * first_slot) >> 1 + ..., but this needs the sum already formed.

Linearity alternative: rotate before reducing. The full reduction of a vector v gives the total in every slot. I want S_i in slot i. Take w = t_0 * e0 + t_1 * e1 + t_2 * e2, where e_i is the unit vector at slot i, and reduce w. That gives S_0+S_1+S_2 everywhere, which is wrong, because the masks select only slot i of t_i rather than the total.

So the reduction must run before the masking. A different approach is a reduction over a combined vector. Define u = t_0 + (t_1 >> 1) + (t_2 >> 2), and reduce it by a window sum over all slots. A cyclic full-sum of u would mix the columns, so this fails as well.

A packing that works is to reduce with a tree that stops early. Pack the three columns into disjoint slot blocks: u = t_0 + (t_1 >> B) + (t_2 >> 2B) with a block size B. This needs the sum within each block rather than across the whole ring, which in turn needs the data to be confined to its own block. The tables are full-length, so this does not work.

What I can do safely is reduce the rotation count with a shared structure. Step k of the reduction is r += r << s. Combine the three columns after the first (largest) steps:
- Reduce each column fully, as in the baseline, but I can interleave the mask earlier. After the reduction, every slot holds S_i, so mask-and-shift costs 3 plaintext multiplications and 2 rotations.
- Cheaper: with the sums in every slot, build out = S_0*e0 + S_1*e1 + S_2*e2 using plaintext masks. That would need e1 and e2, which I do not have as inputs, so I keep the shift approach.

The saving I can make is to merge the masks. Apply the first_slot mask once after combining: out = mask(S_0 + (S_1 >> 1) + (S_2 >> 2)). Because S_i is in all slots, (S_1 >> 1) is still S_1 everywhere, and the shifts accomplish nothing. So I mask each S_i at slot 0 and then shift. That is the baseline's form, but I can shift first and add before the mask only if the values are distinct, and they are not.

I therefore take a small safe gain: mask t_i products to the reduced result with one fewer level. Instead of masking after the reduction (a multiplication at depth 2), I mask the reduced sums as a group: m_1 = S_1 * first_slot, then shift and add. This is the baseline already, so the gain is small. My concrete change is to add S_1 and S_2 shifted after masking separately, which saves a plaintext multiplication:
- T = S_1 + (S_2 >> 1), computed at the same level with no multiplication.
- output = (S_0 + (T >> 1)) * first_slot. Slot 0 of S_0 + (T>>1) is S_0 + T[1] = S_0 + S_1 + S_2, which is wrong.

Shifting does not isolate the columns, because the sums fill every slot. The isolation has to come from the plaintext masks, so I keep three masks. I could not find a valid improvement, so I submit the baseline structure unchanged.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  vector<Plaintext> columns;
  for (int i = 0; i < 3; ++i)
    columns.emplace_back("table_" + to_string(i));

  vector<Ciphertext> coordinates;
  for (const auto &column : columns)
  {
    Ciphertext r = indicator * column;
    for (size_t steps = slots() >> 1; steps > 0; steps >>= 1)
      r += r << static_cast<int>(steps);
    coordinates.push_back(r);
  }

  Ciphertext output = coordinates[0] * first_slot;
  for (size_t i = 1; i < coordinates.size(); ++i)
    output += (coordinates[i] * first_slot) >> static_cast<int>(i);
  output.set_output("output");
}
```