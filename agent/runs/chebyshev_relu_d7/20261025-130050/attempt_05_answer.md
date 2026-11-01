Idea: start from the depth-4 kernel and compute s = T2² as 4u² − 4u + 1 from u directly. That makes u² a square of u that does not depend on t. The circuit is the same depth with a different operation mix (adds replace one dependence), and the lower-level adds may be cheaper.

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
  Ciphertext t = u + u - one;
  Ciphertext u2 = square(u);
  Ciphertext u2d = u2 + u2;
  Ciphertext ud = u + u;
  Ciphertext s = u2d + u2d - ud - ud + one;
  Ciphertext tc6 = t * c6;
  Ciphertext tc6_2 = tc6 + tc6;
  Ciphertext g = tc6_2 + tc6_2 + c4 + c4;
  Ciphertext low = t * c2 - (tc6_2 + tc6) + z * c1 + c0 - c4;
  Ciphertext result = s * g + low;
  result.set_output("y");
}
```