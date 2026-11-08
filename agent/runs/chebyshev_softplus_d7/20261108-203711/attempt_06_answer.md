The circuit has stayed fixed at 3 ct-ct multiplications, 4 plaintext multiplications and 4 levels, and I don't see a cheaper algebraic form. So I'm trying a different operation type for the two products that are pure squarings, z² and T2².

I now write them with `square()` instead of `c * c`. That changes the circuit (square = 2, ct_ct_mul = 1). If CHEHAB maps it to SEAL's dedicated `square`, each product needs 3 component products instead of 4, which may be measurably cheaper.

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
  Ciphertext t2 = zz + zz - one;       // level 2
  Ciphertext t2s = square(t2);         // level 3
  Ciphertext w4 = t2s + t2s;           // level 3, = T4 + 1

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext u2 = u + u;
  Ciphertext inner = u2 + c4;          // level 3
  Ciphertext prod = w4 * inner;        // level 4

  Ciphertext term2 = t2 * c2;          // level 3
  Ciphertext term1 = z * c1;           // level 2
  Ciphertext low = (term2 - u - inner) + term1; // level 3

  Ciphertext result = prod + low + c0;
  result.set_output("y");
}
```