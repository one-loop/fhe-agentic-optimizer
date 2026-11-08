I'm stopping here. I have no new circuit to try, and resubmitting the same kernel only produces duplicates. The only distinct circuits I can build are slower: for example, `(x+x)*x - x*x` equals `x²` but needs two ciphertext multiplications instead of one.

The current circuit, `square(x)`, is already minimal: one multiplication, one relinearization, one rescale and one level. In attempt 1 it was correct and about 2% faster than the baseline (5.99 ms vs 6.10 ms), but that did not reach the 3% needed for confirmation. I'm leaving it as my answer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```