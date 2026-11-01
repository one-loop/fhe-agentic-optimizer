**Idea:** The previous attempt failed because several terms landed on the same level with different multiplication histories. In `c_k * T_k`, the levels are 2, 3, 4, 4, 5, 5, 5, so the additions collide. With seven terms that need distinct levels, the chain depth must be at least 7 for the T's, which is why the baseline's linear recurrence is the right shape. This version keeps the baseline's distinct-level structure: each T_k sits one level above T_{k-1}, so every term has its own level and the additions align for free. It also uses the same six ciphertext multiplications as the baseline, so I don't expect a large speedup. Its purpose is to get a correct kernel back and to avoid the scale-mismatch error.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  vector<Ciphertext> t(8);
  t[1] = z;
  Ciphertext z2 = z * z;
  t[2] = z2 + z2 - one;
  for (int k = 3; k <= 7; ++k)
  {
    Ciphertext zt = z * t[k - 1];
    t[k] = zt + zt - t[k - 2];
  }

  Plaintext c1("c1");
  Ciphertext result = t[1] * c1;
  Plaintext c2("c2");
  result = result + t[2] * c2;
  Plaintext c3("c3");
  result = result + t[3] * c3;
  Plaintext c4("c4");
  result = result + t[4] * c4;
  Plaintext c5("c5");
  result = result + t[5] * c5;
  Plaintext c6("c6");
  result = result + t[6] * c6;
  Plaintext c7("c7");
  result = result + t[7] * c7;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```