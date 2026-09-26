import numpy as np
import argparse

parser = argparse.ArgumentParser(description="Generate I/O examples for nn_avgpool2d benchmark")
parser.add_argument("--slot_count", required=True, type=int, help="Width/height of input image (e.g. 4, 6)", default=4)
args = parser.parse_args()

W = args.slot_count
H = W
kH, kW = 2, 2
stride = 2

# Output shape: H_out = H // stride, W_out = W // stride
out_H = H // stride
out_W = W // stride

np.random.seed(42)
img = np.random.randint(1, 10, size=(H, W))

# Compute 2D Average / Sum Pooling
out = np.zeros((out_H, out_W), dtype=int)
for r in range(out_H):
    for c in range(out_W):
        patch_sum = 0
        for ki in range(kH):
            for kj in range(kW):
                patch_sum += img[r * stride + ki, c * stride + kj]
        out[r, c] = patch_sum

is_cipher = 1
is_signed = 1

with open("fhe_io_example.txt", "w") as file:
    nb_inputs = H * W
    nb_outputs = out_H * out_W
    slot_count = 1
    header = f"{slot_count} {nb_inputs} {nb_outputs}\n"
    file.write(header)

    rows = []
    # Input pixels
    for r in range(H):
        for c in range(W):
            rows.append(f"x_{r}_{c} {is_cipher} {is_signed} {int(img[r, c])}\n")

    # Output pixels
    for r in range(out_H):
        for c in range(out_W):
            rows.append(f"y_{r}_{c} {is_cipher} {int(out[r, c])}\n")

    file.writelines(rows)

print(f"Generated nn_avgpool2d I/O example for image {H}x{W}, pool {kH}x{kW}, stride {stride} -> output {out_H}x{out_W}")
