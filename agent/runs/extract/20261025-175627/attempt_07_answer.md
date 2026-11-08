I have no new valid circuit to try, so this is the baseline kernel again.

The only changes I can think of are moving the rotation before the multiply, rotating the mask, or dropping the mask. CHEHAB rejects the first two or they give wrong values, and the third breaks correctness on the dense case. Splitting the rotation adds work. The baseline's single plaintext multiply and single rotation look minimal.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```