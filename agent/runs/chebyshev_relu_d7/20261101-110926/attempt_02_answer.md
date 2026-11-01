This attempt applies one more Chebyshev identity to remove some operations. Using T4 = 2u² − 1, the top terms satisfy 4c6·u³ = 2v·(T4 + 1), where v = c6·u. The even part then becomes:

E = T4·(2v + c4) + (c2 − c6)·u

This removes the doubling of the product, the separate −c4 plaintext add and one extra copy of v. In exchange it adds one `− one` plaintext add for T4. Expected net change: about 3 fewer ciphertext adds and the same number of plaintext adds. The depth stays at 4, with 3 ciphertext–ciphertext multiplies and 4 ciphertext–plaintext multiplies.

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

  Ciphertext z = x * alpha;          // level 1
  Ciphertext zz = z * z;             // level 2
  Ciphertext u = zz + zz - one;      // T2(z)
  Ciphertext u2 = u * u;             // level 3
  Ciphertext t4 = u2 + u2 - one;     // T4(z)

  Ciphertext v = u * c6;             // c6*u, level 3
  Ciphertext cu = u * c2;            // c2*u, level 3
  Ciphertext q = v + v + c4;         // 2 c6 u + c4
  Ciphertext high = t4 * q;          // level 4

  Ciphertext lin = cu - v;           // (c2 - c6) u
  Ciphertext odd = z * c1;           // level 2

  Ciphertext y = high + lin + odd + c0;
  y.set_output("y");
}
```