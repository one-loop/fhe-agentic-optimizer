This variant computes the c6 term from u instead of s, using c6·s = 2·c6·u − c6. That moves the c6 multiplication one level earlier, where ciphertexts carry more primes but the product is ready sooner, and it trades extra plaintext adds for it.

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
  Ciphertext s = u + u - one;
  Ciphertext q = square(s);
  Ciphertext t4 = q + q - one;
  Ciphertext c6u = u * c6;
  Ciphertext a = c6u + c6u;
  Ciphertext b = a + a;
  Ciphertext d = b + c4 - c6 - c6;
  Ciphertext c2s = s * c2;
  Ciphertext lin = z * c1;
  Ciphertext main_term = t4 * d;
  Ciphertext low = c2s - a;
  Ciphertext result = main_term + low + lin + c6 + c0;
  result.set_output("y");
}
```