I think the attempt 1 kernel is already minimal for this task. I tried several restructurings and none gives a cheaper circuit, so I am resubmitting attempt 1 unchanged.

**Why depth 3 is the floor.** The x² term needs the factor c2·alpha·alpha. Constants cannot be multiplied together as plaintexts, so each of these factors costs its own level, giving depth 3.

**Why there is no cheaper operation count.** The best kernel computes z·(2·c2·z + c1) + c0 − c2 with the minimum work:
- 1 ciphertext×ciphertext multiply;
- 2 scalar plaintext multiplies;
- 3 constant adds;
- 1 ciphertext add;
- 1 mod switch, which aligns z for the final product.

The alternatives I checked are all worse:
- **Square-based form** (z², then multiply by c2 and by c1): 3 plaintext multiplies.
- **Multiplying by x and applying alpha last:** 3 plaintext multiplies and depth 4.
- **Splitting c1·z out:** an extra 5 ms plaintext multiply to save a 2.5 ms add.
- **Pairing x·c2 with x·alpha:** still one alpha short, so it needs extra multiplies.

The circuit is identical to attempt 1, so it will be reported as a duplicate and not timed.

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