The idea is to drop one more plaintext encoding by writing q·inner as (2s)·inner − inner. The product is d2·inner, using the doubled T2 already computed, and inner is subtracted afterwards. This removes the "one" addition that formed q = 2T2 − 1.

The subtraction combines terms at levels 5 and 4, so CHEHAB should align them with a cheap mod switch. Everything else matches attempt 6: 4 ct-ct multiplications, 2 plaintext multiplications and depth 6.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;          // T2
  Ciphertext d2 = s + s;                 // 2 T2

  Ciphertext t = d2 * c7 + c5;           // 2 c7 T2 + c5
  Ciphertext r = s * t - c7;
  Ciphertext inner = r + r + c3;         // 4c7 T2^2 + 2c5 T2 - 2c7 + c3

  Ciphertext m = d2 * inner - inner + c1 - c5 - c7;   // (2T2 - 1) * inner + L
  Ciphertext result = z * m;
  result.set_output("y");
}
```