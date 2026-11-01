This is the last attempt, so I keep the structure of the current best (attempt 6): 3 ct-ct multiplications, 2 plaintext multiplications, no scale alignments. Only the inner term changes. Instead of computing m = 2·t2 and then m·c5, it multiplies t2 by c5 directly and doubles the result:

inner = t2·c5 + t2·c5 + c3 = c5·(4z² − 2) + c3

The one multiplication by c5 now happens on t2, which comes one addition earlier in the chain. m = t2 + t2 is still computed, because n = m − one needs it. The rest is unchanged:

- B = n·inner − c5 + c1
- y = z·B + c0

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = (z2 + z2) - one;   // 2z^2 - 1
  Ciphertext m = t2 + t2;            // 4z^2 - 2
  Ciphertext n = m - one;            // 4z^2 - 3

  Ciphertext t2c = t2 * c5;          // c5 (2z^2 - 1)
  Ciphertext inner = (t2c + t2c) + c3; // c5 (4z^2 - 2) + c3
  Ciphertext B = (n * inner - c5) + c1;
  Ciphertext result = z * B + c0;
  result.set_output("y");
}
```