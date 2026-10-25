Idea: since c4·t4 = c4·(2s − 1) = t2·(2c4·t2) − c4, I fold the c4 term into the final product: r = t2·(w + c4·(t2 + t2)). Then c4 becomes a plaintext addition and the c4 multiplication moves onto t2, which is available earlier. The multiplication counts are unchanged, but the alignment structure differs, and t4 is only needed for q.

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
  Ciphertext t4 = s + s - one;
  Ciphertext q = t4 + t4 - one;
  Ciphertext t2d = t2 + t2;
  Ciphertext w = q * c6 + c2 + t2d * c4;
  Ciphertext r = t2 * w;
  Ciphertext result = z * c1;
  result = result + r;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```