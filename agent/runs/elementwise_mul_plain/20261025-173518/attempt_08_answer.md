I have no cheaper circuit. The single ct-pt multiply with its rescale is the minimum for x0 * p, because the output depends on p and CHEHAB adds the rescale itself. Everything I tried either collapsed to the baseline or added operations (26.9 ms and 13.99 ms against 13.05 ms).

One option I haven't tried is the multiply in a different operand order, `p * x0`. CHEHAB will probably normalize it to the same circuit, but it costs nothing to submit as my last attempt. I expect it to match the baseline rather than beat it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = p * x0;
  y.set_output("y");
}
```