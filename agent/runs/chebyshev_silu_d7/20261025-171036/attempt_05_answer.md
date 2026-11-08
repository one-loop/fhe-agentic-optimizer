Idea: I go back to the working attempt 3 structure and restructure it so it needs no separate `s·c4` term and fewer adds. The even part is t2·(c2 + c4·t2 + c6·A) − c4 + c0, where A = 4s−3 and s = t2². Inside the bracket, `c4·t2` sits at level 3 and `c6·A` at level 4, so they meet at different levels and CHEHAB aligns them for free. The final multiply by t2 lands at level 5. This avoids the same-level history clash that broke attempts 2 and 4.

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext s2 = s + s;
  Ciphertext A = s2 + s2 - one - one - one;
  Ciphertext B = A * c6;
  Ciphertext C = t2 * c4;
  Ciphertext inner = B + C;
  inner = inner + c2;
  Ciphertext D = inner * t2;
  Ciphertext result = D + w * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```