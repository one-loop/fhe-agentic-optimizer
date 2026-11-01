The remaining mixed-level join (n·c3 at L3 against t5·c5 at L4) can be removed by factoring the bracket. Since t5 = m·n − 1:

B = c1 + c3·n + c5·(m·n − 1) = n·(m·c5 + c3) − c5 + c1

The plaintext operations line up as follows:

- **c5** is the only coefficient that needs a plaintext multiplication: m·c5 at L3.
- **c3** is a plaintext add.
- **n·(…)** is a ct-ct multiplication at L4.
- **−c5 and +c1** are plaintext adds.

This leaves 3 ct-ct multiplications and only 2 plaintext multiplications (alpha and c5). Depth stays at 5, and no two ciphertexts with different multiplication histories are added.

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

  Ciphertext inner = m * c5 + c3;    // c5 (4z^2 - 2) + c3
  Ciphertext B = (n * inner - c5) + c1;
  Ciphertext result = z * B + c0;
  result.set_output("y");
}
```