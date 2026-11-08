This is the last attempt. Every structure I found is either identical to attempt 1 or more expensive, so this one is unlikely to win. It is at least a distinct circuit that tests a different alignment pattern instead of resubmitting attempt 1 again.

The idea is to multiply two level-1 ciphertexts (`square(z)`), so the ciphertext×ciphertext multiply needs no mod switch. The cost is a third plaintext multiply (z·c1), about 5 ms, which likely outweighs the ~1 ms mod switch it removes. If it is not faster, attempt 1 remains the best: 17.0 ms against the 24.0 ms baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext q = square(z);
  Ciphertext t = (q + q) * c2;
  Ciphertext l = z * c1;
  Ciphertext y = t + l - c2 + c0;
  y.set_output("y");
}
```