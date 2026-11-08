There is no cheaper exact circuit that I can see. This attempt builds B into a ciphertext at the fresh level and adds it to x*A. It is probably no faster, but it changes how the plaintext add is scheduled relative to the rescale.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext z = x - x;
  z += b;
  Ciphertext y = x * a;
  y += z;
  y.set_output("y");
}
```