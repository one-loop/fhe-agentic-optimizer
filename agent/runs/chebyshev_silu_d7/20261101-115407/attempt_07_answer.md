There is probably no 3% win left in this structure.

- **Already at the floor:** it uses 3 ct-ct multiplications, 3 plaintext multiplications (`alpha`, `c1`, `c6`) and depth 5. Working through the depth-4 cases, they all seem to need a fourth plaintext multiplication to bring in `c2`.
- **Tweaks don't move the time:** varying the doubling, the additions and the alignment placement has stayed within run-to-run noise.

This is a last low-risk variant of attempt 5, which had the shallowest total depth and the best single timing. The constants `c0` and `−c4` are added straight to the deep product e. The shallow correction r = c1·z − 3c6·u, which is already aligned at level 3, is added last. The aim is to change where CHEHAB places its mod switches and alignments.

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
  Ciphertext w = z * z;
  Ciphertext u = w + w - one;          // T2
  Ciphertext v = u + u;                // 2u
  Ciphertext s = u * c6;               // c6 u
  Ciphertext h = s + s + c4;           // 2 c6 u + c4
  Ciphertext mid = v * h + c2;         // 4 c6 u^2 + 2 c4 u + c2
  Ciphertext e = u * mid;              // 4 c6 u^3 + 2 c4 u^2 + c2 u
  Ciphertext ec = e + c0 - c4;
  Ciphertext lin = z * c1;
  Ciphertext r = lin - (s + s + s);    // c1 z - 3 c6 u
  Ciphertext result = ec + r;
  result.set_output("y");
}
```