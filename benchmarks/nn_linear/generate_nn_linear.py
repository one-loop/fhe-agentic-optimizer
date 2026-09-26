import numpy as np
import argparse

# Create argument parser
parser = argparse.ArgumentParser(description="Generate I/O examples for nn_linear benchmark")
parser.add_argument("--slot_count", required=True, type=int, help="Size of input/output features (e.g. 2, 3, 4)", default=4)
args = parser.parse_args()

# In our square benchmark setup, in_features = out_features = slot_count
K = args.slot_count  # in_features
M = args.slot_count  # out_features

# Generate random integer weights, biases, and inputs
np.random.seed(42)
x = np.random.randint(1, 6, size=K)
W = np.random.randint(1, 5, size=(M, K))
b = np.random.randint(1, 4, size=M)

# Linear layer computation: y = W * x + b
y = np.dot(W, x) + b

is_cipher = 1
is_signed = 1

# Write fhe_io_example.txt for CHEHAB
with open("fhe_io_example.txt", "w") as file:
    nb_inputs = K + (M * K) + M
    nb_outputs = M
    slot_count = 1
    header = f"{slot_count} {nb_inputs} {nb_outputs}\n"
    file.write(header)

    rows = []
    # Input vector x
    for j in range(K):
        rows.append(f"x_{j} {is_cipher} {is_signed} {int(x[j])}\n")

    # Weight matrix W
    for i in range(M):
        for j in range(K):
            rows.append(f"w_{i}_{j} {is_cipher} {is_signed} {int(W[i, j])}\n")

    # Bias vector b
    for i in range(M):
        rows.append(f"b_{i} {is_cipher} {is_signed} {int(b[i])}\n")

    # Output vector y
    for i in range(M):
        rows.append(f"y_{i} {is_cipher} {int(y[i])}\n")

    file.writelines(rows)
print(f"Generated nn_linear I/O example for K={K}, M={M}")
