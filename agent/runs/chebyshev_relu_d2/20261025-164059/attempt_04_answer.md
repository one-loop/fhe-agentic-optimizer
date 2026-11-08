I don't see a circuit with fewer operations than attempt 1. It already has the minimum of 1 ciphertext-ciphertext multiplication and 2 plaintext multiplications, because alpha·c2 can't be precomputed. I'm resubmitting that structure with the constant adds applied in a different order. The circuit will probably be normalized to the same one, so this is mainly a check.

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
  Ciphertext w = v + (v + c1);
  Ciphertext r = w * z;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```