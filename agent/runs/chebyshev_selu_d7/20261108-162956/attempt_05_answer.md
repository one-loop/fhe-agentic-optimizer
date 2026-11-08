This attempt regroups the high-degree terms by their shared factor, which is a different circuit from attempt 4:

y = T3·(c3 + 2c4·z + 2c5·T2) + 2·T4·(c6·T2 + c7·T3) + the low-order terms.

The first product reaches level 4, because its bracket tops out at level 3. The second reaches level 5, driven by the level-4 term c7·T3. That is the same total depth and the same count of ciphertext-ciphertext multiplications as attempt 4. The gain I'm aiming for is fewer scale alignments: the bracket for T3 is now shallower, so fewer terms should need alignment. I have not compiled or timed this kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext zc4 = z * c4;
  Ciphertext t2c5 = t2 * c5;
  Ciphertext B = zc4 + zc4;
  B = B + t2c5 + t2c5;
  B = B + c3;
  Ciphertext mainA = t3 * B;

  Ciphertext t2c6 = t2 * c6;
  Ciphertext t3c7 = t3 * c7;
  Ciphertext C = t2c6 + t3c7;
  C = C + C;
  Ciphertext mainB = t4 * C;

  Ciphertext lowz = z * c1;
  lowz = lowz - z * c5;
  lowz = lowz - z * c7;
  Ciphertext lowt = t2 * c2;
  lowt = lowt - t2 * c4;
  lowt = lowt - t2c6;
  Ciphertext low = lowz + lowt;
  low = low + c0;

  Ciphertext result = low + mainA;
  result = result + mainB;
  result.set_output("y");
}
```