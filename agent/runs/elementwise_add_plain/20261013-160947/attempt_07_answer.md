The kernel is the same single plaintext add as before. The 4.84 ms run is noise, since the timings across attempts range from 4.63 to 6.66 ms on identical work. Nothing in the graph can be removed or cheapened, so I'm resubmitting the best-known version.

```cpp
// Elementwise add plain: y = x0 + p. Already minimal (one add, no multiplications,
// no rotations, no level consumption); resubmitted unchanged.

void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```