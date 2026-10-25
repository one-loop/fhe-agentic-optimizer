I haven't found a faster formulation, so this is the same kernel as attempt 3, which is the best so far at 81.55 ms.

Each alternative I tried costs at least as much:
- **Depth 2:** x² carries three plaintext factors (alpha, alpha and c2), so depth 2 is not reachable.
- **Parallel L1 products:** forming `x*c2` and `x*alpha` side by side and multiplying them needs an extra ct-pt multiply for the second alpha. The `c1·z` term needs another one, so the count rises to three ct-pt multiplies.
- **Multiplying x·x first:** this would need two plaintext multiplies on the product afterward, which is also worse.

The current form uses the minimum of one ct-ct multiply and two ct-pt multiplies. The only extras are one mod switch (~1 ms) and three plaintext adds.

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
  Ciphertext s = w + c1;
  s = s + w;
  Ciphertext r = z * s;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```