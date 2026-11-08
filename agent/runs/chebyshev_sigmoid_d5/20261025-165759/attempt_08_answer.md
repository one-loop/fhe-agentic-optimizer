This is the last attempt, and I don't see another structural saving; the remaining plaintext adds are needed for the constants. I'm submitting a small variant of attempt 7 that builds t as u + (u − one), which reuses a subtraction already in the circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext s2 = s + s;
  Ciphertext u = s2 - one;
  Ciphertext um = u - one;
  Ciphertext t = u + um;
  Ciphertext v = u + u;
  Ciphertext m = v * c5;
  Ciphertext r = m + c3;
  Ciphertext P = r * t;
  P = P + c1;
  P = P - c5;
  Ciphertext y = z * P;
  y = y + c0;
  y.set_output("y");
}
```