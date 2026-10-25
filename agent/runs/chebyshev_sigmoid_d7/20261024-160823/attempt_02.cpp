void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;
  // c5*T5 = 2*(t2*c5)*t3 - z*c5
  Ciphertext u5 = (t2 * c5) * t3;
  Ciphertext term5 = u5 + u5 - z * c5;
  Ciphertext term7 = t7 * c7;
  Ciphertext low = z * c1 + t3 * c3;
  Ciphertext result = term7 + (term5 + low);
  result = result + c0;
  result.set_output("y");
}
