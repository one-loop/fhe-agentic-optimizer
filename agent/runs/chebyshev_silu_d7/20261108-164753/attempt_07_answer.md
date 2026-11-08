Attempt 6 gained from moving the c0 add to the end, so I try one more ordering of the final sum. Here `w = z*c1` joins `ms+ms` before `d` does, and c0 is still added last. I have not run this, so it may or may not change the number of alignments.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext m = a + a + c4;
  Ciphertext ms = m * s;
  Ciphertext d = t2 * c2 - m - a;
  Ciphertext w = z * c1;
  Ciphertext result = ((ms + ms) + w) + d;
  result = result + c0;
  result.set_output("y");
}
```