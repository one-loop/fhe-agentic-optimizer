Idea: use `square(z)` for the quadratic term so the ct-ct multiply has a single operand and needs no mod switch. The linear term is `z*c1`, and the doubling is done with an add after the square. I'm not confident this beats attempt 1, because it keeps three plaintext multiplies like attempt 3. The only change from attempt 3 is the explicit `square`, which may count as a different circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext q = s * c2;
  Ciphertext q2 = q + q;
  Ciphertext l = z * c1;
  Ciphertext r = q2 + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```