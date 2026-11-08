The odd coefficients c3, c5, c7 are negligible, so only the even part plus `c1·z` is needed. I evaluate it in the w = T2 basis instead of running the full recurrence:

- w = T2 = 2z² − 1
- T4 = 2w² − 1
- T6 = 2·w·T4 − w

This gives c4·T4 + c6·T6 = T4·(2·c6·w + c4) − c6·w. With u = c6·w, the output becomes:

y = c0 + c1·z + c2·w − u + T4·(2u + c4)

Expected effect versus the baseline:
- Ciphertext-ciphertext multiplications drop from 5 to 3 (z², w², and T4·(2u + c4)).
- Plaintext multiplications drop from 5 to 4.
- Levels drop from 7 to 4.

Same-level additions only combine terms with matching multiplication history, so there should be no scale mismatch. For example, c2·w and u are both w times a plaintext, so they can be subtracted directly.

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
  Ciphertext zz = z * z;
  Ciphertext w = zz + zz - one;          // T2
  Ciphertext ww = w * w;
  Ciphertext t4 = ww + ww - one;         // T4
  Ciphertext u = w * c6;                 // c6*T2
  Ciphertext inner = u + u + c4;         // 2 c6 T2 + c4
  Ciphertext hi = t4 * inner;            // c4 T4 + c6 T6 + c6 T2
  Ciphertext low = w * c2 - u;           // (c2 - c6) T2
  Ciphertext lin = z * c1;
  Ciphertext result = hi + low + lin + c0;
  result.set_output("y");
}
```