void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z + z;
  Ciphertext u = square(z2);
  Ciphertext a = z * c1;
  Ciphertext b = z * c3;
  Ciphertext c = z * c5;
  Ciphertext d = z * c7;
  Ciphertext d2 = d + d;
  Ciphertext d4 = d2 + d2;
  Ciphertext d8 = d4 + d4;
  Ciphertext d7 = d8 - d;
  Ciphertext c4 = (c + c) + (c + c);
  Ciphertext c5x = c4 + c;
  Ciphertext e = c - d7;
  Ciphertext f = (b - c5x) + (d7 + d7);
  Ciphertext S = (a - (b + b + b)) + (c5x - d7);
  Ciphertext h = e + u * d;
  h = f + u * h;
  Ciphertext r = S + u * h;
  r.set_output("y");
}
