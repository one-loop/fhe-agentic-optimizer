# Neural Network Operators Benchmark Suite in CHEHAB

This benchmark suite implements core deep learning operators in **CHEHAB's Domain-Specific Language (DSL)**, adapted from the [Orion](https://github.com/baahl-nyu/orion) FHE deep learning framework.

It forms **Phase 1** of the capstone project:
> **"Optimizing Fully Homomorphic Encryption Code Using LLM-Guided Compiler Feedback"**  
> *Umair Hafeez, Saad Sifar (Advised by Riyadh Baghdadi & Eliseo Ferrante, NYU Abu Dhabi)*

---

## 🧠 Operators Overview

| Operator | Benchmark Folder | Orion Equivalent | Description | Multiplicative Depth |
| :--- | :--- | :--- | :--- | :--- |
| **`Linear`** | `nn_linear/` | `orion.nn.Linear` | Fully Connected / Dense matrix-vector product with bias ($y = Wx + b$) | **1** |
| **`Conv2d`** | `nn_conv2d/` | `orion.nn.Conv2d` | 2D Spatial Convolution with $3 \times 3$ filter and bias ($Y = X \ast K + b$) | **2** |
| **`AvgPool2d`** | `nn_avgpool2d/` | `orion.nn.AvgPool2d` / `AdaptiveAvgPool2d` | 2D Average & Adaptive Average Pooling with stride 2 ($2 \times 2$ downsampling) | **0** |

---

## 📁 Benchmark Anatomy

Each benchmark follows a standardized structure:
```text
benchmarks/<operator_name>/
├── <operator_name>.cpp          # CHEHAB DSL: contains both scalar (fhe) and vectorized (fhe_vectorized)
├── generate_<operator_name>.py  # Generates random test inputs and calculates exact plaintext ground truth
├── fhe_io_example.txt          # Test vector inputs and outputs
├── CMakeLists.txt              # Build configuration for CHEHAB compiler
└── he/
    ├── CMakeLists.txt          # Build configuration linking Microsoft SEAL
    └── main.cpp                # Evaluates encrypted execution, checks correctness, measures noise & latency
```

---

## 🚀 Building the Benchmark Suite

From the root of the `CHEHAB` repository:

```bash
cd /Volumes/SaveHere/capstone/CHEHAB

# Configure and compile all benchmarks
cmake -S . -B build -DCMAKE_PREFIX_PATH="$CONDA_PREFIX"
cmake --build build -j 4
```

---

## 🧪 Running the Operators

Every benchmark is executed in two phases:
1. **Phase 1 (Compiler Optimization & Code Generation):** The CHEHAB compiler analyzes the unoptimized arithmetic circuit, optimizes it using Equality Saturation ($e$-graphs via Rust's `egg`) or Reinforcement Learning (RL), and outputs generated Microsoft SEAL C++ code into `he/`.
2. **Phase 2 (Encrypted Homomorphic Execution):** The generated code is compiled and executed with Microsoft SEAL, performing the computation entirely on ciphertexts, checking output correctness against plaintext math, and measuring execution time and remaining noise budget.

---

### 1. Linear Layer (`nn_linear`)

Computes $y = Wx + b$ for a $4 \times 4$ linear layer:

```bash
cd /Volumes/SaveHere/capstone/CHEHAB/build/benchmarks/nn_linear

# 1. Generate test vectors (in_features = 4, out_features = 4)
python generate_nn_linear.py --slot_count 4

# 2. Phase 1: Compile & optimize with CHEHAB
./nn_linear 1 4 0 0 1 1 1

# 3. Phase 2: Run encrypted execution with Microsoft SEAL
cd he
cmake -S . -B build -DCMAKE_PREFIX_PATH="$CONDA_PREFIX"
cmake --build build -j 4
./build/main
```

**Expected Results:**
- **Optimization:** Initial cost 166,103 $\to$ Final cost 8,203 (~20x cost reduction).
- **Multiplicative Depth:** 1 multiplication (`|mul|: 1`).
- **SEAL Execution:** Remaining noise budget ~327 bits, latency ~60 ms.

---

### 2. 2D Convolution (`nn_conv2d`)

Computes valid 2D convolution with a $3 \times 3$ filter and bias over a $4 \times 4$ image (output size $2 \times 2$):

```bash
cd /Volumes/SaveHere/capstone/CHEHAB/build/benchmarks/nn_conv2d

# 1. Generate test vectors (image width = 4)
python generate_nn_conv2d.py --slot_count 4

# 2. Phase 1: Compile & optimize with CHEHAB
./nn_conv2d 1 4 0 0 1 1 1

# 3. Phase 2: Run encrypted execution with Microsoft SEAL
cd he
cmake -S . -B build -DCMAKE_PREFIX_PATH="$CONDA_PREFIX"
cmake --build build -j 4
./build/main
```

**Expected Results:**
- **Optimization:** Initial cost 367,154 $\to$ Final cost 11,354 (~30x cost reduction).
- **Multiplicative Depth:** 2 multiplications (`|mul|: 2`).
- **SEAL Execution:** Remaining noise budget ~326 bits, latency ~100 ms.

---

### 3. Average Pooling (`nn_avgpool2d`)

Computes $2 \times 2$ average/sum pooling with stride 2 over a $4 \times 4$ image (output size $2 \times 2$):

```bash
cd /Volumes/SaveHere/capstone/CHEHAB/build/benchmarks/nn_avgpool2d

# 1. Generate test vectors (image width = 4)
python generate_nn_avgpool2d.py --slot_count 4

# 2. Phase 1: Compile & optimize with CHEHAB
./nn_avgpool2d 1 4 0 0 1 1 1

# 3. Phase 2: Run encrypted execution with Microsoft SEAL
cd he
cmake -S . -B build -DCMAKE_PREFIX_PATH="$CONDA_PREFIX"
cmake --build build -j 4
./build/main
```

**Expected Results:**
- **Optimization:** Initial cost 5,052 $\to$ Final cost 5,052.
- **Multiplicative Depth:** **0** (`|mul|: 0`, uses additions and rotations only!).
- **SEAL Execution:** Remaining noise budget ~357 bits, latency ~18 ms.

---

## ⚙️ Command-Line Arguments Reference

When running `./<operator_name> <vectorize_code> <slot_count> <opt_method> <window> <quantifier> <cse> <const_folding>`:

| Argument | Description | Recommended Default |
| :--- | :--- | :--- |
| `vectorize_code` | `0` = scalar unvectorized code, `1` = vectorized code | `1` |
| `slot_count` | Dimension size ($N$ for linear, $W$ for spatial maps) | `4` |
| `opt_method` | `0` = $e$-graph equality saturation (`egg`), `1` = Reinforcement Learning | `0` |
| `window` | Vectorization window size (0 for full circuit) | `0` |
| `quantifier` | `1` = print circuit depth, operation counts, and static cost | `1` |
| `cse` | `1` = enable Common Subexpression Elimination | `1` |
| `const_folding` | `1` = enable Constant Folding pass | `1` |

---

## 📊 Automated Batch Benchmark Sweep

To run an automated benchmark sweep across all operators (saving output metrics to CSV under `results/`):

```bash
cd /Volumes/SaveHere/capstone/CHEHAB
python run_benchmarks.py
```

This sweep will automatically measure and record:
- **$C$ (Compilation Time)**
- **$L$ (Execution Latency in ms)**
- **$D$ (Multiplicative Depth & Remaining Noise Budget)**
