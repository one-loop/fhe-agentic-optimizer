The scale mismatch came from two terms meeting at level 4 with different multiplication histories. I now build both the c6 and c4 terms as (pt·w)·(ciphertext), so they have the same history. The c4 term is 2·(c4·w)·w − c4, and the constant −c4 is added as a plaintext. This costs one extra ciphertext multiplication (4 instead of 3), which is still fewer than the baseline's 5. Depth stays 4, and I have not compiled or run this version either.

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
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;          // T2
  Ciphertext s = w * w;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext q = s4 - one - one - one; // 4w^2 - 3
  Ciphertext c6w = w * c6;
  Ciphertext c4w = w * c4;
  Ciphertext w2 = w + w;
  Ciphertext t6 = c6w * q;
  Ciphertext t4 = c4w * w2;
  Ciphertext t2 = w * c2;
  Ciphertext t1 = z * c1;
  Ciphertext result = t6 + t4;
  result = result - c4;
  result = result + t2;
  result = result + t1;
  result = result + c0;
  result.set_output("y");
}
```