I have no structurally cheaper circuit to offer for the last attempt. The 36 products `w_ij * x_j` are independent ciphertexts, each value sits alone in slot 0, and BFV gives no way to pull packed outputs apart again. Everything I tried that changed the circuit (the depth variants) landed within noise of the baseline, and the Winograd version was slower at 39 multiplications.

I'm sending the attempt 6 kernel once more. It is correct, matches the baseline's 36 multiplications and 36 adds, and had the best single timing (1671.97 ms), but it was not confirmed faster, and it will register as a duplicate.

```cpp
void fhe()
{
  const size_t in_features = WIDTH;
  const size_t out_features = WIDTH;

  vector<Ciphertext> x(in_features);
  for (size_t j = 0; j < in_features; ++j)
    x[j] = Ciphertext("x_" + to_string(j));

  vector<vector<Ciphertext>> w(out_features, vector<Ciphertext>(in_features));
  vector<Ciphertext> b(out_features);
  for (size_t i = 0; i < out_features; ++i)
  {
    b[i] = Ciphertext("b_" + to_string(i));
    for (size_t j = 0; j < in_features; ++j)
      w[i][j] = Ciphertext("w_" + to_string(i) + "_" + to_string(j));
  }

  for (size_t i = 0; i < out_features; ++i)
  {
    size_t head = in_features - 2;
    Ciphertext a = b[i];
    for (size_t j = 0; j < head; ++j)
      a += (w[i][j] * x[j]);
    Ciphertext c = w[i][head] * x[head];
    c += (w[i][head + 1] * x[head + 1]);
    Ciphertext y = a + c;
    y.set_output("y_" + to_string(i));
  }
}
```