Idea: restructure the polynomial in v = T2 so the c4 plaintext multiply moves to a shallower level. Using T4 = 2v² − 1 and T6 = v·(4v² − 3), the even part is v·(c2 + c6·w + 2c4·v) − c4, where w = 4v² − 3. The c4 product then acts on v, which sits at a lower level than T4 did, so it is cheaper. This also removes the separate `t4*c4` product.

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
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t4 + t4 - one;
  Ciphertext vc4 = v * c4;
  Ciphertext inner = w * c6 + (vc4 + vc4) + c2;
  Ciphertext result = v * inner - c4;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
```