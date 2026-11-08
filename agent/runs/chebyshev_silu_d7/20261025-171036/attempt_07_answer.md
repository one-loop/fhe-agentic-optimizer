Idea: this is the same algebra as attempt 3 with a cheaper constant handling. Writing A = 2·t4 − 1 with t4 = 2s − 1 needs two plaintext subtractions of `one` instead of three. Doubling t2 before multiplying by c4 replaces one ciphertext add with another, and the doubled value is shared with `t2 = u+u−one`.

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext A = t4 + t4 - one;
  Ciphertext B = A * c6;
  Ciphertext t2d = t2 + t2;
  Ciphertext C = t2d * c4;
  Ciphertext inner = B + C;
  inner = inner + c2;
  Ciphertext D = inner * t2;
  Ciphertext result = D + w * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```