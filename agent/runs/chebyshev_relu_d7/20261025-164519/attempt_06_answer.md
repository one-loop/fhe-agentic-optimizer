Idea: instead of subtracting c6 three times as plaintext adds (about 2.5 ms each) inside the Horner step, subtract 3·w = 3·(c6·s) as ciphertext values after the final multiply. Only the c2 plaintext add remains inside, which removes 3 plaintext adds.

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
  Ciphertext w = s * c6;
  Ciphertext w2 = w + w;
  Ciphertext h = w2 + c4;
  Ciphertext inner = h + h;
  Ciphertext m = s * inner;
  m = m + c2;
  Ciphertext r = s * m;
  r = r - w;
  r = r - w2;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```