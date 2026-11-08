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
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext s = square(w);
  Ciphertext t4 = s + s - one;
  Ciphertext q = w * c6;
  Ciphertext p = q + q + c4;
  Ciphertext low = z * c1;
  Ciphertext mid = w * c2;
  mid = mid - q;
  mid = mid + c0;
  Ciphertext r = p * t4;
  Ciphertext lm = low + mid;
  r = r + lm;
  r.set_output("y");
}
