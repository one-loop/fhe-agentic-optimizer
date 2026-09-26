import numpy as np
import argparse

parser = argparse.ArgumentParser(description="Generate I/O examples for nn_conv2d benchmark")
parser.add_argument("--slot_count", required=True, type=int, help="Width/height of input image (e.g. 4, 6)", default=4)
args = parser.parse_args()

W = args.slot_count
H = W
kH, kW = 3, 3
out_H = H - kH + 1
out_W = W - kW + 1

np.random.seed(42)
img = np.random.randint(1, 5, size=(H, W))
kernel = np.random.randint(1, 4, size=(kH, kW))
bias = np.random.randint(1, 3)

# Compute 2D valid convolution: Y = Conv2D(X, K) + b
out = np.zeros((out_H, out_W), dtype=int)
for r in range(out_H):
    for c in range(out_W):
        window_sum = 0
        for ki in range(kH):
            for kj in range(kW):
                window_sum += kernel[ki, kj] * img[r + ki, c + kj]
        out[r, c] = window_sum + bias

is_cipher = 1
is_signed = 1

with open("fhe_io_example.txt", "w") as file:
    nb_inputs = (H * W) + (kH * kW) + 1
    nb_outputs = out_H * out_W
    slot_count = 1
    header = f"{slot_count} {nb_inputs} {nb_outputs}\n"
    file.write(header)

    rows = []
    # Input pixels
    for r in range(H):
        for c in range(W):
            rows.append(f"x_{r}_{c} {is_cipher} {is_signed} {int(img[r, c])}\n")

    # Kernel weights
    for ki in range(kH):
        for kj in range(kW):
            rows.append(f"k_{ki}_{kj} {is_cipher} {is_signed} {int(kernel[ki, kj])}\n")

    # Bias
    rows.append(f"b {is_cipher} {is_signed} {int(bias)}\n")

    # Output pixels
    for r in range(out_H):
        for c in range(out_W):
            rows.append(f"y_{r}_{c} {is_cipher} {int(out[r, c])}\n")

    file.writelines(rows)

print(f"Generated nn_conv2d I/O example for image {H}x{W}, kernel 3x3 -> output {out_H}x{out_W}")
