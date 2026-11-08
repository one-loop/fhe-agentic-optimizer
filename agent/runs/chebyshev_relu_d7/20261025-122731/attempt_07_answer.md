Idea: keep attempt 2's structure, but compute the c2 term from `u` instead of `t2`. Since c2·t2 = 2·c2·u − c2, the plaintext multiply runs one level earlier, where it is cheaper, and the result lands at the same level as the other terms. The `-c2` joins the constant correction. Unlike attempt 5, I leave the c6 path on `t2`, which keeps the add count low.

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
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext q = t2 * c6;
  Ciphertext r = q + q;
  Ciphertext v = r + c4;
  Ciphertext w = t4 * v;
  Ciphertext ub = u * c2;
  Ciphertext result = z * c1;
  result = result + ub;
  result = result + ub;
  result = result - q;
  result = result + w;
  result = result - c2;
  result = result + c0;
  result.set_output("y");
}
```