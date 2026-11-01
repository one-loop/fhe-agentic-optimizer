The change cuts depth from 6 to 5. It distributes the multiplication by q = 2·T2 − 1, so the final product no longer waits on a chain of three multiplications.

The identity used is:

m = q·(s·g + c3 − 2·c7) + L = (q·s)·g + 2·(s·c3) − 4·(s·c7) − c3 + 2·c7 + L

- **New ct-ct product:** h = q·s (degree 4) is computed in parallel with g, at level 3.
- **Shared term:** q·c7 = 2·e − c7 reuses e = s·c7, so it needs no extra multiplication.
- **Extra cost:** one scalar plaintext multiply, d = s·c3.
- **Scales:** d and e share the same multiplication pattern (s × plaintext) at level 3. The term h·g sits at level 4.

The result is 4 ct-ct multiplications, 3 plaintext multiplications and depth 5.

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
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;          // T2
  Ciphertext q = s + s - one;            // 2T2 - 1

  Ciphertext e = s * c7;
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext g = e4 + c5 + c5;           // 4 c7 T2 + 2 c5
  Ciphertext d = s * c3;

  Ciphertext h = q * s;
  Ciphertext hg = h * g;

  Ciphertext m = hg + d + d - e4 - c3 + c7 + c1 - c5;
  Ciphertext result = z * m;
  result.set_output("y");
}
```