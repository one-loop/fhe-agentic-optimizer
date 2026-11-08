void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext w2 = w + w;
  Ciphertext W = w2 + w2;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext uw4 = u4 * w;
  Ciphertext t = v - u5;
  Ciphertext r = t + uw4;

  Ciphertext m = W * r;

  Ciphertext v2 = v + v;
  Ciphertext low = (s - v2) - t;

  Ciphertext result = low + m;
  result = result + c0;
  result.set_output("y");
}
