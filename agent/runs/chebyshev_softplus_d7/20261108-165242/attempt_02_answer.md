The circuit is already near minimal for plaintext multiplications, because combining c2 and c6 would need plaintext-plaintext arithmetic. So this attempt keeps the same structure and uses `square()` for z² and s² instead of explicit products. Squarings are cheaper than general multiplies, and the circuit counts change, so it counts as a distinct circuit.

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
  Ciphertext u = square(z);
  Ciphertext s = u + u - one;
  Ciphertext q = square(s);
  Ciphertext t4 = q + q - one;
  Ciphertext c6s = s * c6;
  Ciphertext d = c6s + c6s + c4;
  Ciphertext c2s = s * c2;
  Ciphertext lin = z * c1;
  Ciphertext main_term = t4 * d;
  Ciphertext low = c2s - c6s;
  Ciphertext result = main_term + low + lin + c0;
  result.set_output("y");
}
```