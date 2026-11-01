This attempt keeps the depth-5 Horner circuit with 3 ct-ct and 3 ct-pt multiplications. It changes how the −3·c6·u term enters.

Before, `s = c6·u` sat at level 3 and was subtracted at the end. That made the final sum mix three levels: e at L5, s at L3 and lin at L2. Two of the meetings were between different levels.

Now −3c6 is applied as three plaintext additions inside `mid`, before the last multiplication by u, since u·(mid − 3c6) = u·mid − 3s. The final sum then has only two levels, e at L5 and lin at L2. This should remove a scale alignment and some mod switches, at the cost of a few cheap plaintext additions.

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
  Ciphertext mid = u * inner + c2 - c6 - c6 - c6;  // 4c6u^2 + 2c4u + c2 - 3c6
  Ciphertext e = u * mid;              // 4c6u^3 + 2c4u^2 + (c2-3c6)u
  Ciphertext lin = z * c1;
  Ciphertext result = e + lin + c0 - c4;
  result.set_output("y");
}
```