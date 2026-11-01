The baseline already does the minimum work for this function: one plaintext multiply and one rotation. The output needs data moved across slots, so at least one rotation is unavoidable, and one plaintext multiply is needed to apply the mask. The current order (multiply, then rotate) also performs the rotation after the rescale, so it runs at the cheaper lower level. Rotating the input first would run at full level and cost more. Since there is no fused or cheaper rotation primitive in the DSL, I see no legitimate speedup. I'm returning the same computation, with the rotation written explicitly as a single shift.

```cpp
// Same computation as the baseline: one plaintext multiply (mask projection),
// then one rotation. Rotating after the multiply means the rotation runs at the
// already-rescaled (lower) level, which is the cheapest valid ordering.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
```