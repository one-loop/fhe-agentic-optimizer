The circuit already looks close to minimal, so I'm trying one more variant and don't expect much gain.

- **Ciphertext multiplications (3):** z², u·inner and u·mid. The degree-3 term in u needs two multiplications in u, plus one to form w = z².
- **Plaintext multiplications (3):** α, c1 and c6. c2 and c4 already enter as plaintext additions. Working through the possible depth-4 products, c2 cannot reach the u-term without its own plaintext multiplication, so depth 5 is needed with 3 plaintext multiplications.

The change is in how the doubling is placed. Instead of doubling h at level 3 (inner = h + h), I double u at level 2 (v = u + u) and compute mid = v·h + c2. That should change where CHEHAB places its alignments.

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
  Ciphertext lin = z * c1;
  Ciphertext r = lin - (s + s + s);    // c1 z - 3 c6 u
  Ciphertext result = (e + r) + c0 - c4;
  result.set_output("y");
}
```