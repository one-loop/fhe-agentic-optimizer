This is the attempt-6 kernel with one change: the factor 2 is applied to b before the product instead of after it. So q = u²·(2b) at level 4 is formed directly. That drops the doubling add at level 4, and the extra add now happens at level 3. The algebra, the three plaintext additions, the `square()` calls and the add order are otherwise unchanged:

y = u²·(2b) − b + (c2·u − a) + c1·z + c0, with a = c6·u and b = 2a + c4.

This is a marginal circuit change. It may not clear the 3% bar over attempt 1.

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

  Ciphertext z = x * alpha;          // level 1
  Ciphertext l1 = z * c1;            // level 2
  Ciphertext z2 = square(z);         // level 2
  Ciphertext u = z2 + z2 - one;      // T2, level 2
  Ciphertext u2 = square(u);         // level 3
  Ciphertext a = u * c6;             // level 3
  Ciphertext b = a + a + c4;         // level 3
  Ciphertext b2 = b + b;             // level 3
  Ciphertext q = u2 * b2;            // level 4
  Ciphertext lin = u * c2 - a - b;   // level 3
  Ciphertext s = q + lin;
  Ciphertext s2 = s + l1;
  Ciphertext result = s2 + c0;
  result.set_output("y");
}
```