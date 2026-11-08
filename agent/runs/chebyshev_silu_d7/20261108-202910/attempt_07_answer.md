I don't see a structurally cheaper circuit. The kernel uses 3 ciphertext multiplications, the minimum for an even degree-6 part. It uses 4 plaintext multiplications, which are forced because alpha, c1, c2 and c6 are independent and plaintext-plaintext products are not allowed. Depth is 4, which also looks minimal.

The only overhead left is the two scale alignments CHEHAB inserts, and reordering the additions in attempts 2–5 didn't remove them. None of those reorderings was confirmed faster than attempt 1.

This attempt takes the explicit-T4 form of attempt 2 and computes both self-products with `square()`. SEAL's squaring is cheaper than a general multiply, and this operation mix hasn't been timed yet, so it is a distinct circuit. I expect at most a small gain, likely under the 3% needed to count as a new best.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext u = square(z);            // level 2
  Ciphertext t2 = u + u - one;         // level 2
  Ciphertext s = square(t2);           // level 3
  Ciphertext t4 = s + s - one;         // level 3

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext high = t4 * w;            // level 4

  Ciphertext lin = z * c1 + c0;        // level 2
  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext low = (c2t2 - c6t2) + lin; // level 3

  Ciphertext result = low + high;
  result.set_output("y");
}
```