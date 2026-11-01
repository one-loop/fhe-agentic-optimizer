This attempt keeps attempt 2's circuit: Horner in u = T2, with 3 ct-ct and 3 ct-pt multiplications at depth 5. Only the order of the final additions changes. I don't expect a large gain.

- **Before:** the −3·c6·u correction (three copies of s) was subtracted from the deep product e at level 5.
- **Now:** it is first combined with the odd term c1·z, so r = c1·z − 3s. Both are shallow, at levels 2 and 3, and should align more cheaply there.
- **Final step:** that sum is added to e, then the constants c0 and −c4 are added last, at the deepest and cheapest level.

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
  Ciphertext s = u * c6;               // c6 u
  Ciphertext h = s + s + c4;           // 2 c6 u + c4
  Ciphertext inner = h + h;            // 4 c6 u + 2 c4
  Ciphertext mid = u * inner + c2;     // 4 c6 u^2 + 2 c4 u + c2
  Ciphertext e = u * mid;              // 4 c6 u^3 + 2 c4 u^2 + c2 u
  Ciphertext lin = z * c1;
  Ciphertext r = lin - (s + s + s);    // c1 z - 3 c6 u
  Ciphertext result = (e + r) + c0 - c4;
  result.set_output("y");
}
```