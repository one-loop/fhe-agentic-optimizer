Attempt 3 was slower, so I'm going back to the attempt 2 structure. This time I only reorder the final sums so that terms at the same level are combined before the level-4 product is added. That should reduce the number of level and scale alignments CHEHAB has to insert.

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
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext s = square(w);
  Ciphertext t4 = s + s - one;
  Ciphertext q = w * c6;
  Ciphertext p = q + q + c4;
  Ciphertext low = z * c1;
  low = low + c0;
  Ciphertext mid = w * c2;
  mid = mid - q;
  Ciphertext r = p * t4;
  Ciphertext lm = low + mid;
  r = r + lm;
  r.set_output("y");
}
```