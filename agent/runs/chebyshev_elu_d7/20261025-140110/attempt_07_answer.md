Idea: I keep the attempt-4 chain (zd = 2z multiplied directly) for the middle steps. The first step is multiplied by z and doubled afterwards (a cheap add instead of a plaintext-level double). The last steps reuse z so that no extra aligned doubling is needed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zd = z + z;
  Ciphertext p6 = z * c7;
  Ciphertext b6 = (p6 + p6) + c6;
  Ciphertext b5 = (zd * b6 + c5) - c7;
  Ciphertext b4 = (zd * b5 + c4) - b6;
  Ciphertext b3 = (zd * b4 + c3) - b5;
  Ciphertext b2 = (zd * b3 + c2) - b4;
  Ciphertext p1 = z * b2;
  Ciphertext b1 = ((p1 + p1) + c1) - b3;
  Ciphertext r = (z * b1 + c0) - b2;
  r.set_output("y");
}
```