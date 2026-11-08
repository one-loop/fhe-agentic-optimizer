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
  Ciphertext t44 = t22 + t22;                // T4 + 1, level 3

  // odd part: T1 * [ t44*(2r) + (u c3 - u c5 - 2r) + c1 - c7 ], r = c5 + c7 u
  Ciphertext r = u * c7 + c5;                // level 3
  Ciphertext r2 = r + r;
  Ciphertext qq = u * c3 - u * c5 - r2;      // level 3
  Ciphertext m = t44 * r2;                   // level 4
  Ciphertext br = m + qq + c1 - c7;          // level 4
  Ciphertext odd = t1 * br;                  // level 5

  // even part: t44*g + (c2 T2 - c6 T2 - g), g = 2 c6 T2 + c4
  Ciphertext c6t2 = t2 * c6;                 // level 3
  Ciphertext g = c6t2 + c6t2 + c4;
  Ciphertext e4 = t44 * g;                   // level 4
  Ciphertext e3 = t2 * c2 - c6t2 - g;        // level 3
  Ciphertext even = e4 + e3;                 // level 4

  Ciphertext result = odd + even + c0;       // level 5
  result.set_output("y");
}
