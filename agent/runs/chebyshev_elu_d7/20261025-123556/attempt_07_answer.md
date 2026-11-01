Attempt 6's algebra checks out, so I can't tell what produced the wrong output. The shared-polynomial form is probably tripping something in how CHEHAB handles plaintext adds. I'll take a more conservative step.

- The even part E goes back to the verified attempt-5 form.
- The odd bracket keeps the shared-polynomial form for A3 and A5, which save 3 plaintext multiplies.
- A7 is built negated, as −A7 = 4v² + 4v − 8v³ − 1, using only a subtraction of `one`. It is then subtracted after the multiply, so no `+ one` appears.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext v = u + u - one;
  Ciphertext v2 = square(v);
  Ciphertext v3 = v * v2;

  Ciphertext vv = v + v;
  Ciphertext v2x2 = v2 + v2;
  Ciphertext v2x4 = v2x2 + v2x2;
  Ciphertext v3x2 = v3 + v3;
  Ciphertext v3x4 = v3x2 + v3x2;
  Ciphertext v3x8 = v3x4 + v3x4;

  Ciphertext A3 = vv - one;
  Ciphertext A5 = v2x4 - vv - one;
  Ciphertext nA7 = v2x4 + vv - v3x8 - one;

  Ciphertext B = A3 * c3;
  B = B + A5 * c5;
  B = B - nA7 * c7;
  B = B + c1;

  // Even part (verified form)
  Ciphertext v_c2 = v * c2;
  Ciphertext v_c6 = v * c6;
  Ciphertext w_c4 = v2 * c4;
  Ciphertext x_c6 = v3 * c6;
  Ciphertext e1 = v_c2 - v_c6 - v_c6 - v_c6;
  Ciphertext e2 = w_c4 + w_c4;
  Ciphertext e3 = x_c6 + x_c6;
  e3 = e3 + e3;
  Ciphertext E = e1 + e2;
  E = E + e3;
  E = E + c0;
  E = E - c4;

  Ciphertext zB = z * B;
  Ciphertext result = zB + E;
  result.set_output("y");
}
```