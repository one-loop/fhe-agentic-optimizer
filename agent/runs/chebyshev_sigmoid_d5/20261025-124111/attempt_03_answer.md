This variant keeps the t2-factored form from attempt 2 but adds the constant c1 − c3 − c5 to the inner sum as plaintext adds instead of three extra plaintext multiplies of z. It uses 3 ct-ct multiplies (including the square) and 2 plaintext multiplies (alpha and c5), compared with 3 plaintext multiplies in attempt 1. The depth goes back up to 5.

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
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext a = t2 + t2 - one;
  Ciphertext w = a + a;
  Ciphertext m = w * c5 + c3 + c3;
  Ciphertext inner = t2 * m + c1 - c3 - c5;
  Ciphertext result = inner * z + c0;
  result.set_output("y");
}
```