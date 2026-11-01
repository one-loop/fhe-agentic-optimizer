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
  Ciphertext m = q * t3;
  Ciphertext m2 = m + m;
  Ciphertext lin = z * c1 - z * c5 - z * c7;
  Ciphertext result = lin + t3 * c3;
  result = result + m2;
  result = result + c0;
  result.set_output("y");
}
