This is a last structural variant. I fold P0, P1 and the full sum P0+P1+P2, then recover t2 by subtraction after the masks. Operation counts should match the baseline's, so I do not expect it to be faster. I found no way to share the reduction rotations across columns without extra masks.

```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  Plaintext first_slot("first_slot");
  Plaintext t0("table_0"), t1("table_1"), t2("table_2");

  Ciphertext p0 = indicator * t0;
  Ciphertext p1 = indicator * t1;
  Ciphertext p2 = indicator * t2;
  Ciphertext s = p0 + p1;
  s += p2;

  auto fold = [](Ciphertext r) {
    for (size_t steps = 1; steps < slots(); steps <<= 1)
      r += r << static_cast<int>(steps);
    return r;
  };

  Ciphertext m0 = fold(p0) * first_slot;
  Ciphertext m1 = fold(p1) * first_slot;
  Ciphertext ms = fold(s) * first_slot;
  Ciphertext m01 = m0 + m1;
  Ciphertext m2 = ms - m01;

  Ciphertext acc = m2 >> 1;
  acc += m1;
  acc = acc >> 1;
  acc += m0;
  acc.set_output("output");
}
```