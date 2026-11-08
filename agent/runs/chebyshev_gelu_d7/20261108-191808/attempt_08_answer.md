This is the last attempt. The algebra has stayed at the same floor in every try: 4 levels, 3 ct-ct multiplications, 4 plaintext multiplications, 2 alignments. Placement changes so far have given 1–2%, short of the 3% needed.

One placement not yet tried: add c0 to the level-3 low part, and add the level-2 term c1·z separately at the very end. CHEHAB then aligns c1·z directly to level 4 in one step, and nothing extra rides on the level-3-to-4 alignment. Everything else stays the same: T4·w = 2(u²·w) − w, so T4 is never formed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext zz = z * z;               // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext uu = u * u;               // level 3
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext p = uu * w;               // level 4
  Ciphertext mid = u * c2 - c6u - w + c0;   // level 3
  Ciphertext lin = z * c1;             // level 2
  Ciphertext result = p + p + mid + lin;
  result.set_output("y");
}
```