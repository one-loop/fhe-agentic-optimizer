Idea: drop the plaintext subtraction that builds t. Since t = v − 1 with v = 2u, P = t·r + c1 − c5 = v·r − r + c1 − c5. The ciphertext subtraction of r costs about 0.5 ms, whereas building t cost a plaintext op.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext u = s + s;
  u = u - one;
  Ciphertext v = u + u;
  Ciphertext m = u * c5;
  Ciphertext m2 = m + m;
  Ciphertext r = m2 + c3;
  Ciphertext P = v * r;
  P = P - r;
  P = P + c1;
  P = P - c5;
  Ciphertext y = z * P;
  y = y + c0;
  y.set_output("y");
}
```