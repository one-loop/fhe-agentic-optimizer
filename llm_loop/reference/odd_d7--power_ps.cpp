// Reference for degree-7 Chebyshev kernels with odd terms and c0 (sigmoid),
// 4-level variant: power basis, Paterson-Stockmeyer split in z^2 / z^4.
// T3 = 4z^3 - 3z, T5 = 16z^5 - 20z^3 + 5z, T7 = 64z^7 - 112z^5 + 56z^3 - 7z, so
//   y = c0 + (A1 + z^2 A3) + z^4 (A5 + z^2 A7), A_k = a_k z,
//   a1 = c1 - 3c3 + 5c5 - 7c7, a3 = 4c3 - 20c5 + 56c7, a5 = 16c5 - 112c7, a7 = 64c7.
// The integer combinations of c_j z use additions only (no plaintext arithmetic).
// Expected: 4 levels, 5 ct x ct, 5 ct x pt (alpha, c1, c3, c5, c7).

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
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext c1z = z * c1, c3z = z * c3, c5z = z * c5, c7z = z * c7;
  Ciphertext a1 = combo({{1, c1z}, {-3, c3z}, {5, c5z}, {-7, c7z}});
  Ciphertext a3 = combo({{4, c3z}, {-20, c5z}, {56, c7z}});
  Ciphertext a5 = combo({{16, c5z}, {-112, c7z}});
  Ciphertext a7 = combo({{64, c7z}});
  Ciphertext z2 = z * z;
  Ciphertext z4 = z2 * z2;
  Ciphertext low = a1 + z2 * a3;
  Ciphertext high = a5 + z2 * a7;
  Ciphertext y = low + z4 * high + c0;
  y.set_output("y");
}
