void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext W = x * z;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext uW = u * W;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext r = (v - u5) + uW;

  Ciphertext m = W * r;

  Ciphertext v2 = v + v;
  Ciphertext v3 = v2 + v;
  Ciphertext low = (s - v3) + u5;

  Ciphertext result = low + m;
  result = result + c0;
  result.set_output("y");
}
