Use reduce_add.
```cpp
void fhe()
{
  Ciphertext indicator("indicator");
  for (int i = 0; i < 3; ++i)
  {
    Plaintext column("table_" + to_string(i));
    reduce_add(indicator * column).set_output("output_" + to_string(i));
  }
}
```
