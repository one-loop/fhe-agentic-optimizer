Attempt 6 confirmed that moving the ciphertext multiply onto x is slower: 46.36 ms against 41.52 ms. The extra plaintext multiply and the extra level cost more than the alignment it saved.

I don't see any remaining structure that beats attempt 1:

- **Depth:** depth 3 is the minimum. The x² coefficient needs c2·alpha·alpha, and each plaintext factor costs its own level.
- **Plaintext multiplies:** every other split (x·c2 in parallel with x·alpha, a separate c1·z, or squaring first) needs 3 or more.
- **Constants:** the 3 plaintext adds are needed because plaintext-only combinations are not allowed.
- **Alignment:** the remaining mod switch costs only about 1 ms.

I am returning the attempt 1 kernel as the final answer. It will be reported as identical and not timed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext w = z * c2;             // level 2: c2*z
  Ciphertext inner = w + w + c1;     // 2*c2*z + c1
  Ciphertext prod = inner * z;       // level 3
  Ciphertext result = prod + c0 - c2;
  result.set_output("y");
}
```