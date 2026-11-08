With one attempt left, I don't see a cheaper circuit. The kernel is at the minimum I found: 3 ciphertext multiplications, 4 plaintext multiplications (alpha, c1, c2, c6) and 4 levels. Reordering the additions has not removed the two scale alignments CHEHAB inserts.

This last try is a placement variant of attempt 4, not a new structure, and I expect at most a small change. The constant c0 now joins the level-3 group (c2·T2 − c6·T2 − w + c0), and the bare c1·z term is added to that group last. The aim is that the alignment for the linear term costs a cheap mod switch rather than a multiply-plain plus rescale.

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
  Ciphertext s = square(t2);           // level 3 (T4 = 2s - 1)

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext sw = s * w;               // level 4
  Ciphertext high = sw + sw;           // T4*w + w

  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext mid = c2t2 - c6t2 - w + c0; // level 3

  Ciphertext lin = z * c1;             // level 2
  Ciphertext low = mid + lin;

  Ciphertext result = high + low;
  result.set_output("y");
}
```