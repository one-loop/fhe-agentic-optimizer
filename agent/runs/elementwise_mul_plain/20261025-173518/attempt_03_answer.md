The last attempt confirmed that extra multiplies cost more (26.9 ms against 13.05 ms). One ct-pt multiply is the floor for x0 * p. Any cheaper circuit would have to drop the multiply or the rescale, and CHEHAB inserts the rescale automatically.

I could not find a correct circuit with fewer operations. The ct-pt multiply cannot be removed, because the output depends on p. The rescale cannot be removed, because CHEHAB adds it after every multiply. I'm resubmitting the best kernel so I don't get a slower variant.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```