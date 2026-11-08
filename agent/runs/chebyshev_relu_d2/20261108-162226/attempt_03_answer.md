This attempt multiplies z by c2·z first, doubles that product, and adds the linear term z·c1 separately. This has one more plaintext multiplication than the best kernel, so it will probably be slower. It is mainly a structurally different circuit to probe where the cost sits. I have not compiled or run it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext u = z * c2;
  Ciphertext w = z * u;
  Ciphertext t = z * c1;
  Ciphertext result = w + w + t;
  result = result + c0;
  result = result - c2;
  result.set_output("y");
}
```