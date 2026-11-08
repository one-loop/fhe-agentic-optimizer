In the best kernel (attempt 6) the factor 2 in the c5 term was added as a plaintext twice. Each plaintext add costs about 2.5 ms because of encoding. Here I build 2·c7·T2 + c5 with one plaintext add and then double the ciphertext, which gives the same value with one fewer plaintext add. Everything else is unchanged. I have not compiled or timed it.

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
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext m = t2 * c7;
  m = m + m;
  m = m + c5;
  m = m + m;
  Ciphertext inner = t2 * m;
  inner = inner + c3;
  inner = inner - c7;
  inner = inner - c7;
  Ciphertext q = u * inner;
  q = q + c1;
  q = q - c5;
  q = q - c7;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
```