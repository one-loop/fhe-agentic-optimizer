void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;                 // level 1
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;             // level 2
  Ciphertext u = t2 + t2 - one;              // 2*T2 - 1, level 2
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;           // level 3

  // odd part: T1 * [(c1 - c7) + u*(c3 - c5) + 2*T4*(c5 + c7*u)]
  Ciphertext r = u * c7 + c5;                // level 3
  Ciphertext m = t4 * r;                     // level 4
  Ciphertext q = u * c3 - u * c5 + c1 - c7;  // level 3
  Ciphertext br = m + m + q;                 // level 4
  Ciphertext odd = t1 * br;                  // level 5

  // even part: T4*(2 c6 T2 + c4) + c2 T2 - c6 T2 + c0
  Ciphertext c6t2 = t2 * c6;                 // level 3
  Ciphertext g = c6t2 + c6t2 + c4;
  Ciphertext e4 = t4 * g;                    // level 4
  Ciphertext e3 = t2 * c2 - c6t2;            // level 3
  Ciphertext even = e4 + e3 + c0;            // level 4

  Ciphertext result = odd + even;
  result.set_output("y");
}
