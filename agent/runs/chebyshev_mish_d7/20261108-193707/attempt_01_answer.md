**Idea:** use a baby-step / giant-step split with T4 as the giant step. I build only T1, T2, T3 and T4 from the products z², z·T2 and T2². The high-degree terms then come from the identity T_{4+j} = 2·T4·T_j − T_{4−j}:

- c5·T5 = c5·(2·T4·T1 − T3)
- c6·T6 = c6·(2·T4·T2 − T2)
- c7·T7 = c7·(2·T4·T3 − T1)

So y = c0 + c1T1 + c2T2 + c3T3 + c4T4 + 2·T4·(c5T1 + c6T2 + c7T3) − (c5T3 + c6T2 + c7T1).

**Expected effect:**

| | Baseline | This kernel |
|---|---|---|
| ct-ct multiplications | 6 | 4 |
| ct-pt multiplications | 8 | 10 (9 distinct, since c6·T2 is reused) |
| Multiplicative depth | 8 | 5 |

Every operation should also get cheaper because the kernel consumes fewer levels.

**Risk:** terms meet at the same level when they are added, which could trigger a scale mismatch. I chose groupings that I expect to share the same multiplication pattern, but CHEHAB has not run this yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;                 // level 1
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;             // level 2
  Ciphertext zt2 = t1 * t2;
  Ciphertext t3 = zt2 + zt2 - t1;            // level 3
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;           // level 3

  Ciphertext c6t2 = t2 * c6;
  Ciphertext a = t1 * c5 + c6t2 + t3 * c7;   // up to level 4
  Ciphertext b = t3 * c5 + c6t2 + t1 * c7;

  Ciphertext w = t4 * a;                     // level 5
  Ciphertext high = w + w - b;

  Ciphertext low = t1 * c1 + t2 * c2 + t3 * c3 + t4 * c4;
  Ciphertext result = low + high + c0;
  result.set_output("y");
}
```