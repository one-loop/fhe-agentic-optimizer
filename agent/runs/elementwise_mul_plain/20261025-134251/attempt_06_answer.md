That confirmed it: two plaintext multiplies cost about twice as much (14.43 ms vs 7.16 ms). The baseline's single multiply plus one rescale is the minimum for x0 * p, and I can't remove any operation from it. Any different circuit adds operations, so I'm submitting the baseline kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```