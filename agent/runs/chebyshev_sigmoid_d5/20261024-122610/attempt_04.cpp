void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = z * u;
  Ciphertext s = w * c5;
  Ciphertext su = s * u;
  Ciphertext a2 = su + su;
  Ciphertext a4 = a2 + a2;
  Ciphertext a8 = a4 + a4;
  Ciphertext a16 = a8 + a8;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext s16 = s4 + s4;
  s16 = s16 + s16;
  Ciphertext s20 = s16 + s4;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext z2 = z + z;
  Ciphertext z4 = z2 + z2;
  Ciphertext z3x = z4 - z;
  Ciphertext z5 = z4 + z;
  Ciphertext e3 = w4 - z3x;
  Ciphertext r = z * c1 + e3 * c3 + z5 * c5 + a16 - s20 + c0;
  r.set_output("y");
}
