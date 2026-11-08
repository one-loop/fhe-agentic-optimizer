The mismatch was between the c3·T3 + c1·z terms and the c5·T5 term, which both ended at level 4. I fold c3 in earlier too, as 2·(z·c3)·T2 − z·c3, so that group ends at level 3. The three groups then end at levels 3, 4 and 5, and none of the additions meets operands at the same level. This attempt is also uncompiled, so the same kind of scale mismatch could still show up elsewhere.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext zc3 = z * c3;
  Ciphertext p3 = zc3 * t2;
  Ciphertext s3 = p3 + p3 - zc3;
  Ciphertext a5 = t2 * c5;
  Ciphertext p5 = a5 * t3;
  Ciphertext s5 = p5 + p5 - z * c5;
  Ciphertext a7 = t3 * c7;
  Ciphertext p7 = a7 * t4;
  Ciphertext s7 = p7 + p7 - z * c7;
  Ciphertext low = z * c1 + s3;
  Ciphertext mid = low + s5;
  Ciphertext result = mid + s7;
  result.set_output("y");
}
```