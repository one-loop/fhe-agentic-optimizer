This is the last attempt, and the counts appear to be at their floor: 5 ciphertext-ciphertext and 5 plaintext multiplications, depth 4, and 3 unavoidable level-2 to level-3/4 alignments. So the only change from attempt 7 is how u = 2·T2 = 4z² − 2 is built.

I double z at level 1, which is a cheap addition, then square it to get 4z² directly. Subtracting "one" twice gives u, replacing the two doubling adds at level 2. Everything else is unchanged: (u·u)·q at level 4, plus a level-3 group where lin is aligned once.

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
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z1 = z * c1;
  Ciphertext z3 = z * c3;
  Ciphertext z5 = z * c5;
  Ciphertext z7 = z * c7;

  Ciphertext z2 = z + z;
  Ciphertext w4 = z2 * z2;
  Ciphertext u = w4 - one - one;
  Ciphertext uu = u * u;

  Ciphertext p2 = u * z7;
  Ciphertext q = p2 + (z5 - z7);
  Ciphertext g4 = uu * q;

  Ciphertext r = z3 - z5;
  Ciphertext a2 = u * r;
  Ciphertext lin = z1 + z7 - z3 - z5;
  Ciphertext mid = (a2 - p2 - p2) + lin;

  Ciphertext result = g4 + mid + c0;
  result.set_output("y");
}
```