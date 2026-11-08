Idea: reorder the linear term so the ct-pt multiplications happen in a different order. I compute x·c1 first and then multiply by alpha, and I build the sum so that terms at the same level are added together before alignment. The polynomial is unchanged.

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
  Ciphertext q = square(u);
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext m = q * b;
  Ciphertext lin = (x * c1) * alpha;
  Ciphertext mid = u * c2 - (a2 + a);
  Ciphertext r = (m + m) + mid;
  r = r + lin;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```