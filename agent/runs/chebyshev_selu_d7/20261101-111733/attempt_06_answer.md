Attempt 5 was 2% faster than the best, short of the 3% needed. This keeps that structure and changes only the two self-products: z·z and T2·T2 become `square(...)`.

In SEAL, squaring a ciphertext costs one fewer polynomial product than a general multiply. The circuit is unchanged except that two ct-ct multiplications become squares.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext T1 = x * alpha;
  Ciphertext zz = square(T1);
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext T3 = T1 * u;
  Ciphertext t22 = square(T2);

  Ciphertext a5 = T1 * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext q = a7 + a6 + a5;
  Ciphertext inner = q + q + c4;

  Ciphertext P = t22 * inner;

  Ciphertext l1 = T1 * c1 - T1 * c7;
  Ciphertext l2 = T2 * c2 - a6;
  Ciphertext l3 = T3 * c3 - T3 * c5;
  Ciphertext low = l3 + l2 + l1;

  Ciphertext rest = low - inner;
  Ciphertext result = P + P + rest;
  result = result + c0;
  result.set_output("y");
}
```