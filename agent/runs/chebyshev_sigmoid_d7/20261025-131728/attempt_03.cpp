void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = z * z;
  Ciphertext w = s + s - one;
  Ciphertext zw = z * w;
  Ciphertext t3 = zw + zw - z;
  Ciphertext ww = w * w;
  Ciphertext t4 = ww + ww - one;
  Ciphertext q = w * c5 + t4 * c7;
  Ciphertext r = q + q + c3;
  Ciphertext m = r * t3;
  Ciphertext lin = z * c1 - z * c5 - z * c7;
  Ciphertext result = lin + m;
  result = result + c0;
  result.set_output("y");
}
