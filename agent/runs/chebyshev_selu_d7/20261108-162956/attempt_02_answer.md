Attempt 1 failed because c3·T3 and c4·T4 met at level 4 with different scale histories. I now compute T4 = 2·T3·z − T2, which puts it at level 4. The terms then meet as follows:
- c3·T3 is at level 4.
- c4·T4, c5·T5 and c6·T6 meet at level 5, all as ct-ct product times plaintext.
- c7·T7 is at level 6.
- c1·z (level 2) and c2·T2 (level 3) also sit at different levels from the rest.

This costs one extra level compared with attempt 1, 6 in total against the baseline's 8. I have not compiled or timed this kernel.

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
  Ciphertext a4 = z * t3;
  Ciphertext t4 = a4 + a4 - t2;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext t3sq = square(t3);
  Ciphertext t6 = t3sq + t3sq - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext low = z * c1;
  low = low + t2 * c2;
  low = low + t3 * c3;
  Ciphertext mid = t4 * c4;
  mid = mid + t5 * c5;
  mid = mid + t6 * c6;
  Ciphertext result = mid + low;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```