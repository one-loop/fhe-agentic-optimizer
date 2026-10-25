Idea: instead of forming T6 = 2·T2·T4 − T2 and then multiplying by c6, multiply c6 into T2 first (a cheap plaintext multiplication at a shallow level) and then by T4. This gives c6·T6 = 2·(c6·T2)·T4 − c6·T2. The circuit keeps 3 ciphertext multiplications and 5 plaintext multiplications, but the depth drops from 5 to 4, so the later operations run at deeper, cheaper levels.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext a6 = t2 * c6;
  Ciphertext p = a6 * t4;
  Ciphertext term6 = p + p - a6;
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + term6;
  result = result + c0;
  result.set_output("y");
}
```