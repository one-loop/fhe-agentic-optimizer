I don't have a valid kernel with a cheaper circuit. `x0 * x1` is already the minimum: one ct-ct multiply, one relinearization and one rescale.

I checked three alternatives, and each fails:
- **Square identity:** `((x0+x1)² - (x0-x1)²)/4` needs two squares plus a 1/4 constant. CKKS forbids numeric constants, and no plaintext input supplies one. It also costs more.
- **Cheaper multiplication at a deeper level:** this would need a mod switch or plaintext multiply on the inputs first. That adds alignment work and consumes a level, so it costs more than it saves.
- **Algebraic padding:** adding zero terms or reordering is normalized away by CHEHAB, so the circuit stays identical to the baseline.

I'm resubmitting the minimal kernel. It will match the baseline circuit and can't be ranked as faster.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```