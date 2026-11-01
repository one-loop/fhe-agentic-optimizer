Idea: this is the attempt-1 structure with the plaintext additions reordered and the doubling moved onto `z`. CHEHAB probably normalizes this to the same circuit, so I don't expect a gain. I haven't found a cheaper circuit that avoids plaintext-plaintext arithmetic or an extra level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = z * c2;
  Ciphertext w = (v + v) + c1;
  Ciphertext m = z * w;
  Ciphertext r = m - c2;
  r = r + c0;
  r.set_output("y");
}
```