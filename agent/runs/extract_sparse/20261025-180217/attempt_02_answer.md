The idea is to factor the mask so both outputs come from fewer distinct terms: `out0 = ((first - second) * drop + second) << n` and `out1 = (second * drop) << n`. The subtraction happens before the multiply, at a cheap level. This is mathematically the same as before, and the operation counts may come out slightly different after CHEHAB normalizes the graph.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext d = first - second;
  Ciphertext t = d * drop_front;
  Ciphertext b = second * drop_front;
  Ciphertext s = t + second;
  Ciphertext out0 = s << num_dense;
  Ciphertext out1 = b << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```