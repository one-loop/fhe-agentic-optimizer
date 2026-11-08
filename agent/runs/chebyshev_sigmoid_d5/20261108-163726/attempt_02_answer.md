The previous kernel built 4w from w with extra doublings. Since z = x/4, the product W = x·z equals 4z² directly, so it can stand in for 4w. That removes the doubling adds on the uw and m terms (4 fewer ciphertext additions). The circuit is otherwise the same: 3 ciphertext-ciphertext multiplications and 4 plaintext multiplications. I haven't run it, so the timing and accuracy are untested.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext W = x * z;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext uW = u * W;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext r = (v - u5) + uW;

  Ciphertext m = W * r;

  Ciphertext v2 = v + v;
  Ciphertext v3 = v2 + v;
  Ciphertext low = (s - v3) + u5;

  Ciphertext result = low + m;
  result = result + c0;
  result.set_output("y");
}
```