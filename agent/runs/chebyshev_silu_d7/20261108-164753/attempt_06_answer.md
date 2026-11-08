The previous kernels all hit the minimum of 3 ciphertext multiplications for a degree-6 polynomial, so the remaining gain is in the alignment steps. Here I change the order of the final sum. First `ms+ms` meets `d` (level 3), and then `w = z*c1` (level 2) comes in. The c0 add moves to the last step, where the ciphertext carries the fewest primes and the plaintext encoding is cheapest. I have not run this ordering, so I cannot say yet whether it reduces the alignments.

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
  Ciphertext result = ((ms + ms) + d) + w;
  result = result + c0;
  result.set_output("y");
}
```