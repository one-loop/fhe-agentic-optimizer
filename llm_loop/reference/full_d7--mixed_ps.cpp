// Reference for degree-7 Chebyshev kernels with every coefficient c0..c7
// (elu, selu, mish), 4-level variant: the odd part in the power basis with a
// Paterson-Stockmeyer split (as odd_d7--power_ps.cpp) and the even part in
// u = T2(z) (as even_d7--cheb_u.cpp), with u^2 = 4 z^4 - 4 z^2 + 1 taken from
// the z^4 the odd part needs anyway:
//   y = [c0 - c4 + (c2 u - 3 c6 u) + 2 u^2 (c4 + 2 c6 u)] + (A1 + z^2 A3) + z^4 (A5 + z^2 A7).
// Expected: 4 levels, 6 ct x ct, 7 ct x pt (alpha, c1, c3, c5, c7, c2, c6).

// k * v for an integer k >= 1, by doubling.
Ciphertext times(const Ciphertext &v, int k)
{
  Ciphertext result, power = v;
  bool has = false;
  for (; k > 0; k >>= 1)
  {
    if (k & 1)
    {
      result = has ? result + power : power;
      has = true;
    }
    if (k > 1)
      power = power + power;
  }
  return result;
}

// sum_i k_i v_i for nonzero integers k_i.
Ciphertext combo(const vector<pair<int, Ciphertext>> &terms)
{
  Ciphertext pos, neg;
  bool has_pos = false, has_neg = false;
  for (const auto &[k, v] : terms)
  {
    if (k > 0)
    {
      pos = has_pos ? pos + times(v, k) : times(v, k);
      has_pos = true;
    }
    else
    {
      neg = has_neg ? neg + times(v, -k) : times(v, -k);
      has_neg = true;
    }
  }
  return has_neg ? pos - neg : pos;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext c1z = z * c1, c3z = z * c3, c5z = z * c5, c7z = z * c7;
  Ciphertext a1 = combo({{1, c1z}, {-3, c3z}, {5, c5z}, {-7, c7z}});
  Ciphertext a3 = combo({{4, c3z}, {-20, c5z}, {56, c7z}});
  Ciphertext a5 = combo({{16, c5z}, {-112, c7z}});
  Ciphertext a7 = combo({{64, c7z}});
  Ciphertext z2 = z * z;
  Ciphertext z4 = z2 * z2;
  Ciphertext odd = (a1 + z2 * a3) + z4 * (a5 + z2 * a7);

  Ciphertext u = z2 + z2 - one;
  Ciphertext uu = times(z4, 4) - times(z2, 4) + one;
  Ciphertext c6u = u * c6;
  Ciphertext even_inner = c6u + c6u + c4;
  Ciphertext even_high = uu * even_inner;
  Ciphertext even = (even_high + even_high) + (u * c2 - (c6u + c6u + c6u)) + c0 - c4;

  Ciphertext y = even + odd;
  y.set_output("y");
}
