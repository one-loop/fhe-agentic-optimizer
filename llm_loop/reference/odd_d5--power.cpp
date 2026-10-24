// Reference for degree-5 Chebyshev kernels with odd terms and c0 (sigmoid d5),
// power basis: T3 = 4z^3 - 3z, T5 = 16z^5 - 20z^3 + 5z, so
//   y = c0 + A1 + z^2 (A3 + z^2 A5), A_k = a_k z,
//   a1 = c1 - 3c3 + 5c5, a3 = 4c3 - 20c5, a5 = 16c5.
// Expected: 4 levels, 3 ct x ct, 4 ct x pt (alpha, c1, c3, c5).

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
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext c1z = z * c1, c3z = z * c3, c5z = z * c5;
  Ciphertext a1 = combo({{1, c1z}, {-3, c3z}, {5, c5z}});
  Ciphertext a3 = combo({{4, c3z}, {-20, c5z}});
  Ciphertext a5 = combo({{16, c5z}});
  Ciphertext z2 = z * z;
  Ciphertext y = a1 + z2 * (a3 + z2 * a5) + c0;
  y.set_output("y");
}
