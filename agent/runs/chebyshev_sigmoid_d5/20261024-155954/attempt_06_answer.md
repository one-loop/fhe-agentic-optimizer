Idea: scale u by 4 once (u4 = 4u, two additions) instead of scaling several other terms. Since u·inner = u4·(pq + u4·q), the inner term needs no ×4 or ×16 doublings. It is still 3 ct-ct multiplications and 4 plaintext multiplications, with fewer ciphertext additions.

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
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
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