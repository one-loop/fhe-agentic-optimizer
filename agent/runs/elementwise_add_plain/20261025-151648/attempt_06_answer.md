I still have no cheaper circuit to offer. The remaining options all fail for the reasons I gave earlier:

- CHEHAB normalizes away any redundant rewrite.
- Any extra operation that survives normalization costs more than the add itself.
- Only a deeper-level encoding could be cheaper, and I can't reach a deeper level without paying more than it saves.

I'm submitting the single-add kernel again rather than inventing a slower variant. The verdict will likely be "identical to the baseline" once more.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```