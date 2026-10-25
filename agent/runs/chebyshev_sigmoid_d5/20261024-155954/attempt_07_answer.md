Idea: u only appears as u4 = 4u, and 4z² = (2z)². So I compute z2 = z + z and u4 = square(z2) directly, which drops the intermediate u and one addition. It is still 3 ct-ct multiplications, with the squaring operating on the same level.

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
  Ciphertext z2 = z + z;
  Ciphertext u4 = square(z2);
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext pq = p - q4 - q;
  Ciphertext p2 = p + p;
  Ciphertext A = c1z - p2 - pq;
  Ciphertext inner = pq + u4 * q;
  Ciphertext y = A + u4 * inner;
  y = y + c0;
  y.set_output("y");
}
```