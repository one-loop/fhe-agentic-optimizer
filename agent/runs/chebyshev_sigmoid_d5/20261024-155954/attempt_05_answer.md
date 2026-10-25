Idea: the circuit is at its floor in ct-ct and plaintext multiplications. I'm making a small change by taking the "×4" after the u·q product (and as one chain) instead of before, so more of the cheap additions happen after the multiplication. This gives a new circuit shape with the same multiplication counts.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext uq = u * q;
  Ciphertext uq2 = uq + uq;
  Ciphertext uq4 = uq2 + uq2;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext pq = p - q4 - q;
  Ciphertext p2 = p + p;
  Ciphertext A = c1z - p2 - pq;
  Ciphertext inner4 = pq + uq4;
  Ciphertext inner2 = inner4 + inner4;
  Ciphertext inner = inner2 + inner2;
  Ciphertext y = A + u * inner;
  y = y + c0;
  y.set_output("y");
}
```