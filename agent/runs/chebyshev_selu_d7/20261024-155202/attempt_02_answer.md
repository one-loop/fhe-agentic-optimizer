I split the polynomial into even and odd parts in s = T2(z), so that y = E(s) + z·O(s). This should need 7 plaintext multiplications instead of 9, with the same 4 ciphertext multiplications.

- **Even part:** E = c0 + c2·s + c4·q + c6·r, where q = T2(s) = T4(z) and r = T3(s) = T6(z).
- **Odd part:** O = c1 + c3·V1 + c5·V2 + c7·V3, where T_{2k+1}(z) = z·V_k:
  - V1 = 2s − 1
  - V2 = 2q − V1
  - V3 = 2r − V2

Depth rises to 6, so the saving from fewer plaintext multiplications and alignments has to outweigh the extra level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;
  Ciphertext s2 = s * s;
  Ciphertext q = s2 + s2 - one;
  Ciphertext sq = s * q;
  Ciphertext r = sq + sq - s;
  Ciphertext v1 = s + s - one;
  Ciphertext v2 = q + q - v1;
  Ciphertext v3 = r + r - v2;
  Ciphertext e = s * c2 + q * c4 + r * c6 + c0;
  Ciphertext o = v1 * c3 + v2 * c5 + v3 * c7 + c1;
  Ciphertext y = e + z * o;
  y.set_output("y");
}
```