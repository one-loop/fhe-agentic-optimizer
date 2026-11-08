I should correct my last message: the kernel I submitted was `a + a - a`, with no split multiply and no plaintext add as I described. It only added two ciphertext adds, and it came out slower (14.03 ms vs 13.67 ms).

I still don't see a cheaper circuit. The output is a single product of the ciphertext and the plaintext vector. That needs one plaintext multiply, and CHEHAB inserts the rescale itself. Rotations, extra multiplies and add/sub tricks only add work. Constants and plaintext-plaintext arithmetic are not allowed, so nothing can replace the multiply. The baseline appears to be the minimal circuit, so I'm resubmitting it unchanged. It will not be timed again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```