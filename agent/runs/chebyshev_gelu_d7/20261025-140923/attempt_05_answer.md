Idea: use Horner form in u so the c2·u plaintext multiplication disappears. The polynomial becomes:

y = c0 − c4 + c1·z + u·(c2 + 2·u·b) − 3a

where a = c6·u and b = c4 + 2a. This still uses 3 ct-ct multiplications (s, u·b, u·w) but only 3 plaintext multiplications, because c2 enters as a plaintext add instead. The cost is one extra level of depth.

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
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext ub = u * b;
  Ciphertext w = (ub + ub) + c2;
  Ciphertext r = u * w;
  r = r + z * c1;
  r = r - (a2 + a);
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```