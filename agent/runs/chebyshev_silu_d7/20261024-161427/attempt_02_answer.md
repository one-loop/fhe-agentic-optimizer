Idea: put the even part in terms of t2 = 2u − 1 and s = t2². Then T4 = 2s − 1 and T6 = t2·(4s − 3), so

even = c0 + c4·(2s − 1) + t2·(c2 + c6·(4s − 3)).

This needs the same 3 ciphertext-ciphertext multiplications (u, s, and t2·w). It drops one plaintext multiplication, because T4 and T6 are no longer built as separate ciphertexts, and it removes some alignment work.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext q = s4 - one - one - one;
  Ciphertext w = q * c6 + c2;
  Ciphertext t4 = s2 - one;
  Ciphertext r = t2 * w;
  Ciphertext result = z * c1;
  result = result + t4 * c4;
  result = result + r;
  result = result + c0;
  result.set_output("y");
}
```