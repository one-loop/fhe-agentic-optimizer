This version cuts one level, from 6 to 5. It keeps 4 ciphertext-ciphertext multiplications, at the cost of 2 more scalar plaintext multiplications.

The bracket is expanded as a cubic in T2: B = b0 + b1·T2 + b2·T2² + b3·T2³, where

- b0 = c1 − c3 − c5 + c7
- b1 = 2c3 − 2c5 − 4c7
- b2 = 4c5 − 4c7
- b3 = 8c7

It is evaluated Paterson-Stockmeyer style:

- The low part, b0 + b1·T2, is built from T2·c3, T2·c5 and T2·c7, all at level 3.
- The high part, 4·T2²·(c5 − c7 + 2c7·T2), uses T2² at level 3 and one product at level 4.
- Finally y = c0 + z·B at level 5.

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
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zs = z * z;
  Ciphertext t2 = zs + zs - one;          // T2 (L2)
  Ciphertext t2sq = t2 * t2;              // T2^2 (L3)
  Ciphertext e = t2 * c7;                 // c7 T2 (L3)
  Ciphertext h = e + e + c5 - c7;         // c5 - c7 + 2c7 T2
  Ciphertext hp = t2sq * h;               // (L4)
  Ciphertext hp2 = hp + hp;
  Ciphertext high = hp2 + hp2;            // 4 * T2^2 * h
  Ciphertext a3 = t2 * c3;
  Ciphertext a5 = t2 * c5;
  Ciphertext l = a3 - a5 - e - e;
  Ciphertext low = l + l;                 // (2c3 - 2c5 - 4c7) T2
  Ciphertext b = high + low + c1 - c3 - c5 + c7;
  Ciphertext r = z * b;                   // (L5)
  Ciphertext y = r + c0;
  y.set_output("y");
}
```