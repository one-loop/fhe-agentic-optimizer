Every higher-order term contains T3, so I can factor it out:

c3·T3 + c5·T5 + c7·T7 = T3·(c3 + 2·c5·T2 + 2·c7·T4) − (c5 + c7)·z

The z-terms are combined as c1·z − c5·z − c7·z, which avoids any plaintext-plaintext arithmetic. This drops the circuit to 4 ciphertext-ciphertext multiplications (w, T3, T2², and the single product T3·inner). The terms c5·T2 and c7·T4 sit at different levels, so scale alignment should be free. I have not compiled or timed this version.

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
  Ciphertext t3 = z * u;
  Ciphertext s2 = t2 * t2;
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c5;
  Ciphertext b = t4 * c7;
  Ciphertext inner = a + b;
  inner = inner + inner;
  inner = inner + c3;
  Ciphertext big = t3 * inner;
  Ciphertext zt = z * c1 - z * c5 - z * c7;
  Ciphertext result = big + zt;
  result = result + c0;
  result.set_output("y");
}
```