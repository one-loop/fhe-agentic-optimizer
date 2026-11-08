The structure and counts seem to be at a floor: 4 levels, 3 ct-ct multiplications, 4 plaintext multiplications. So this attempt changes which operation evaluates the two self-products.

CHEHAB currently emits z·z and u·u as generic ciphertext multiplications (square=0 in the counts). Writing them as `square()` should map them to SEAL's dedicated squaring, which is somewhat cheaper. It also gives a distinct circuit. Everything else is the attempt-4 kernel.

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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext zz = square(z);           // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext uu = square(u);           // level 3
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext p = uu * w;               // level 4
  Ciphertext mid = u * c2 - c6u - w;   // level 3
  Ciphertext lin = z * c1;             // level 2
  Ciphertext high = p + p + mid;       // level 4
  Ciphertext result = (high + lin) + c0;
  result.set_output("y");
}
```