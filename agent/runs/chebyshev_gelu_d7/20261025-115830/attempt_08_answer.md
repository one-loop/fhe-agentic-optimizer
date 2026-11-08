Idea: this is the attempt 7 structure with the cheap terms combined earlier. I add c0 and c2·(nothing extra) to the low-level partial sums, so they are aligned once at a shallow level. The only deep product, w·inner, is then added last. The circuit shape is otherwise the same, with 3 ct-ct multiplications and 4 plaintext multiplications.

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
  Ciphertext g = z * c1;
  g = g + c0;
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext s = square(w);
  Ciphertext v = s + s - one;
  Ciphertext b = v + v - one;
  Ciphertext inner = b * c6;
  inner = inner + c2;
  Ciphertext r = v * c4;
  r = r + g;
  Ciphertext f = w * inner;
  Ciphertext result = f + r;
  result.set_output("y");
}
```