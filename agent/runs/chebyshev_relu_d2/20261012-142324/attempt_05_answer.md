I'm stopping here rather than spending more attempts. The last four runs of this kernel measured 41.3–41.4 ms, so the differences are noise. I have no structurally different variant that would beat it. The remaining cost is one ciphertext×ciphertext multiply (about 30 ms, the floor for a degree-2 polynomial), plus about 11 ms of scalar multiplies, plaintext additions and one mod switch.

The kernel below is the attempt-1 version, which is the fastest measured run (41.32 ms). It is correct on every case, with max error about 1e-7 against the 1e-6 tolerance.

```cpp
// Horner form of the degree-2 Chebyshev approximation in z = x/4:
//   y = c0 - c2 + z * (2*c2*z + c1)
// One ciphertext-ciphertext multiply, two scalar plaintext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");

  Ciphertext z = x * alpha;          // z in [-1, 1], depth 1
  Ciphertext a = z * c2;             // c2 * z, depth 2
  Ciphertext b = a + a;              // 2 * c2 * z (same level, no alignment)
  Ciphertext inner = b + c1;         // 2 c2 z + c1, depth 2
  Ciphertext p = inner * z;          // z * (2 c2 z + c1), depth 3
  Ciphertext q = p - c2;             // subtract c2
  Ciphertext y = q + c0;             // add c0
  y.set_output("y");
}
```