Attempt 5 is the same kernel again, measuring 168.12 ms. The spread across attempts 3–5 (167.67 to 168.12 ms) is run-to-run noise, so the design has plateaued.

I see no further structural savings worth testing. Each T_k still needs at least one ciphertext-plaintext multiply, and the four ciphertext-ciphertext multiplies are the minimum for this degree. I'm returning the attempt 3 kernel as the final answer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // Chebyshev basis
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // group A: (c1 - c7) T1   (level 2)
  Ciphertext A1 = z * c1;
  Ciphertext A2 = z * c7;
  Ciphertext A = A1 - A2;

  // group B: (c2 - c6) T2   (level 3)
  Ciphertext B1 = T2 * c2;
  Ciphertext B2 = T2 * c6;
  Ciphertext B = B1 - B2;

  // group C: (c3 - c5) T3   (level 4)
  Ciphertext C1 = T3 * c3;
  Ciphertext C2 = T3 * c5;
  Ciphertext C = C1 - C2;

  Ciphertext low = A + B;
  low = low + C;

  // q = c5 T1 + c6 T2 + c7 T3
  Ciphertext q1 = z * c5;
  Ciphertext q2 = T2 * c6;
  Ciphertext q3 = T3 * c7;
  Ciphertext q = q1 + q2;
  q = q + q3;

  // T4 * (2q + c4)  == 2 T4 q + c4 T4
  Ciphertext qq = q + q;
  qq = qq + c4;
  Ciphertext P = T4 * qq;

  Ciphertext result = P + low;
  result = result + c0;
  result.set_output("y");
}
```