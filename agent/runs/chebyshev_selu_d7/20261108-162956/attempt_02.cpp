void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext a4 = z * t3;
  Ciphertext t4 = a4 + a4 - t2;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext t3sq = square(t3);
  Ciphertext t6 = t3sq + t3sq - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext low = z * c1;
  low = low + t2 * c2;
  low = low + t3 * c3;
  Ciphertext mid = t4 * c4;
  mid = mid + t5 * c5;
  mid = mid + t6 * c6;
  Ciphertext result = mid + low;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
