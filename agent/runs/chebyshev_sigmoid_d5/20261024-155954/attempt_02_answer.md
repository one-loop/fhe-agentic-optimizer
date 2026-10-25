Idea: keep the folded form y = c0 + A + u·(B + u·D) and factor the 4 out of the inner term. Since B = 4(p − 5q) and D = 16q, inner = 4·((p − 5q) + u·(4q)). This keeps 3 ct-ct multiplications, 4 plaintext multiplications and depth 4, and saves a few ciphertext additions. The doubling of the inner term is also done on a ciphertext that sits at a lower level, so it is cheaper.

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
  Ciphertext u = z * z;
  Ciphertext c1z = z * c1;
  Ciphertext p = z * c3;
  Ciphertext q = z * c5;
  Ciphertext p3 = p + p + p;
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext q5 = q4 + q;
  Ciphertext A = c1z - p3 + q5;
  Ciphertext pq = p - q5;
  Ciphertext inner4 = pq + u * q4;
  Ciphertext inner2 = inner4 + inner4;
  Ciphertext inner = inner2 + inner2;
  Ciphertext y = A + u * inner;
  y = y + c0;
  y.set_output("y");
}
```