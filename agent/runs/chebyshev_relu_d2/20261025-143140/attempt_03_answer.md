I did not find a circuit that beats the current best. Every way I tried to cut a multiplication ended up needing a third plaintext multiplication to apply α. The kernel below is the same minimal form with only the add structure rearranged, so CHEHAB will probably normalize it to the same circuit as attempt 1.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext u = (w + c1) + w;
  Ciphertext p = z * u;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
```