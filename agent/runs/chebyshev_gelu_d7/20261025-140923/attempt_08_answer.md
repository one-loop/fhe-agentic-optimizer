Last attempt: I combine the two tweaks from attempts 6 and 7, doubling u up front and summing the side terms before adding u·w. The operation counts are the same as the best kernel, so any gain would come from cheaper alignment and ordering.

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
  Ciphertext s = square(z);
  Ciphertext u = s + s - one;
  Ciphertext u2 = u + u;
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext w = (u2 * b) + c2;
  Ciphertext side = z * c1;
  side = side - (a2 + a);
  side = side + c0;
  side = side - c4;
  Ciphertext r = u * w;
  r = r + side;
  r.set_output("y");
}
```