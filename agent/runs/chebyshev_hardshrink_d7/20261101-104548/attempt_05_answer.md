The idea is to cut plaintext additions, which cost about 2.5 ms each, while keeping the circuit of attempt 3. Attempt 4 showed that dropping a level does not pay off when it adds plaintext multiplications and scale matches.

The inner factor gets a factor of 2 pulled out:

inner = 2·[s·(2e + c5) − c7] + c3, where e = s·c7 and s = T2

This adds c5 once instead of twice, and c7 once instead of twice. Plaintext additions go from 10 to 8. The counts of ciphertext-ciphertext multiplications, plaintext multiplications and levels stay the same.

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
  Ciphertext q = s + s - one;            // 2T2 - 1

  Ciphertext e = s * c7;
  Ciphertext t = e + e + c5;             // 2 c7 T2 + c5
  Ciphertext r = s * t - c7;
  Ciphertext inner = r + r + c3;         // 4c7 T2^2 + 2c5 T2 - 2c7 + c3

  Ciphertext m = q * inner + c1 - c5 - c7;
  Ciphertext result = z * m;
  result.set_output("y");
}
```