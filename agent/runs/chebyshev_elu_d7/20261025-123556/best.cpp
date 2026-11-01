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

  Ciphertext v_c2 = v * c2;
  Ciphertext v_c3 = v * c3;
  Ciphertext v_c5 = v * c5;
  Ciphertext v_c6 = v * c6;
  Ciphertext v_c7 = v * c7;
  Ciphertext w_c4 = v2 * c4;
  Ciphertext w_c5 = v2 * c5;
  Ciphertext w_c7 = v2 * c7;
  Ciphertext x_c6 = v3 * c6;
  Ciphertext x_c7 = v3 * c7;

  Ciphertext e1 = v_c2 - v_c6 - v_c6 - v_c6;
  Ciphertext e2 = w_c4 + w_c4;
  Ciphertext e3 = x_c6 + x_c6;
  e3 = e3 + e3;
  Ciphertext E = e1 + e2;
  E = E + e3;
  E = E + c0;
  E = E - c4;

  Ciphertext b1 = v_c3 + v_c3 - v_c5 - v_c5 - v_c7 - v_c7 - v_c7 - v_c7;
  Ciphertext b2 = w_c5 - w_c7;
  b2 = b2 + b2;
  b2 = b2 + b2;
  Ciphertext b3 = x_c7 + x_c7;
  b3 = b3 + b3;
  b3 = b3 + b3;
  Ciphertext B = b1 + b2;
  B = B + b3;
  B = B + c1;
  B = B - c3;
  B = B - c5;
  B = B + c7;

  Ciphertext zB = z * B;
  Ciphertext result = zB + E;
  result.set_output("y");
}
