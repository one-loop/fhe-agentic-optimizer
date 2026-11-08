I still see no way to get below one ct-ct and two ct-pt multiplications. This attempt takes a different route: x·c2 and x·α are computed in parallel at level 1, then the α factor is applied to the doubled c2·x term. It uses an extra plaintext multiplication, but both first-level products are independent, so it shows whether the shallower dependency chain helps.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = x * c2;
  Ciphertext u = ((v + v) * alpha) + c1;
  Ciphertext p = z * u;
  Ciphertext y = (p + c0) - c2;
  y.set_output("y");
}
```