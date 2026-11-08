Same result as before: the baseline is the cheapest possible circuit, and this attempt resubmits it unchanged.

Computing `x0 * s` with `s` as a runtime plaintext needs one ct-pt multiply. CKKS then requires one rescale, which CHEHAB inserts automatically and the DSL cannot suppress. There are no rotations, ct-ct multiplies, additions or alignments left to remove, and the circuit already uses the minimum of one level. Any different circuit adds operations and is slower, so none of the 4 remaining attempts can produce a valid improvement. I recommend stopping.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```