Idea: halve the Horner inner term once more. Computing t·(2·c6·t + c4) and then doubling the result saves a plaintext add (about 2.5 ms of encoding) and one ciphertext add.

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
  Ciphertext t = u + u - one;
  Ciphertext w = t * c6;
  Ciphertext w2 = w + w;
  Ciphertext inner = w2 + c4;
  Ciphertext mh = t * inner;
  Ciphertext m = mh + mh;
  Ciphertext g = m + c2 - c6 - c6 - c6;
  Ciphertext r = t * g + z * c1;
  Ciphertext result = r + c0 - c4;
  result.set_output("y");
}
```