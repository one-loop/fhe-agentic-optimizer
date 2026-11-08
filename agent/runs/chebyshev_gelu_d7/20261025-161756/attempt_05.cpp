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
  Ciphertext q = w * c6;
  Ciphertext p = q + q + c4;
  Ciphertext low = z * c1;
  low = low + c0;
  Ciphertext mid = w * c2;
  mid = mid - q;
  mid = mid - p;
  Ciphertext ps = p * s;
  Ciphertext r = ps + ps;
  Ciphertext lm = low + mid;
  r = r + lm;
  r.set_output("y");
}
