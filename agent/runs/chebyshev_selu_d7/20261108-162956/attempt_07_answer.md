I'm returning to the structure of attempt 4, since folding c6 into a single product keeps colliding with c7·T4 at level 4. This time I split the work across two products so each bracket has only one level-4 term:
- T3·(c3 + 2c4·z + 2c5·T2 + 2c6·T3)
- T4·(2c7·T3)

Nine ciphertext-plaintext multiplications result instead of 10, with 5 ciphertext-ciphertext multiplications as before. The risk is that the two level-5 products have different scale histories when added. I have not compiled or timed this kernel.

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
  Ciphertext t3c6 = t3 * c6;
  Ciphertext B = zc4 + zc4;
  B = B + t2c5 + t2c5;
  B = B + c3;
  Ciphertext hi = t3c6 + t3c6;
  B = B + hi;
  Ciphertext mainA = t3 * B;

  Ciphertext t3c7 = t3 * c7;
  Ciphertext C = t3c7 + t3c7;
  Ciphertext mainB = t4 * C;

  Ciphertext lowz = z * c1;
  lowz = lowz - z * c5;
  lowz = lowz - z * c7;
  Ciphertext lowt = t2 * c2;
  lowt = lowt - t2 * c4;
  Ciphertext low = lowz + lowt;
  low = low + c0;
  low = low - c6;

  Ciphertext result = low + mainA;
  result = result + mainB;
  result.set_output("y");
}
```