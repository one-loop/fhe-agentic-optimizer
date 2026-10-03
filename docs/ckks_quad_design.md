# Minimum CKKS compiler path for Quad: design note

Status: design only, no code changed. Base: `chehab-ckks-operator-integration`
(src/ identical to upstream `main` @ `4c5c367`). Oracle: the direct-SEAL module in
`benchmarks/dl_operators_ckks/` (accepted baseline).

Target path:

```
CHEHAB DSL -> Compiler::compile -> CHEHAB passes -> CKKS SEAL codegen
  -> generated program build -> encrypted execution -> correctness check
```

Target program (milestone 1, nothing else):

```cpp
Ciphertext x("x");
auto y = x * x;
y.set_output("y");
```

Out of scope: BatchNorm, Chebyshev, vectorizer, e-graph, RL, LLM integration,
optimization, automatic CKKS parameter selection.

## 0. What CHEHAB does today with this exact program

Compiled through the current (BFV) path with CSE, operand ordering and constant
folding on, `simplification_ruleset`, `gen_he_code` defaults:

```cpp
Ciphertext c1 = encrypted_inputs.at("x");
evaluator.multiply(c1, c1, c1);
evaluator.relinearize(c1, relin_keys, c1);
encrypted_outputs.emplace("y", move(c1));
```

Quantifier: depth (1, 1), one relin. The DSL, TRS, CSE and relin pass already
handle Quad correctly. `x * x` stays a `mul` (it is not turned into `square`).
For CKKS, `fhe()` needs exactly one extra line,
`evaluator.rescale_to_next(c1, c1);`, plus a CKKS runtime around it. Everything
below exists to produce that line and that runtime without disturbing BFV.

## 1. Where should BFV/CKKS scheme selection live?

**In `ir::Func`, fixed when the function is created.** The scheme is a property
of the program, like `slot_count`, not a global compiler mode, and every
consumer (passes, Quantifier, code generation) already receives the `Func`.

| File | Change |
|---|---|
| `src/fheco/dsl/common.hpp` | add `enum class Scheme { bfv, ckks };` and `struct CkksParams { std::size_t poly_modulus_degree; std::vector<int> coeff_mod_bit_sizes; int log2_scale; };` next to the existing `SecurityLevel` |
| `src/fheco/ir/func.hpp/.cpp` | add `Scheme scheme_ = Scheme::bfv;` and `std::optional<CkksParams> ckks_params_;` with getters; add one new constructor for CKKS; existing constructors unchanged and default to BFV |
| `src/fheco/dsl/compiler.hpp` | add `Compiler::create_ckks_func(std::string name, std::size_t slot_count, CkksParams params, bool need_cyclic_rotation = false)`. A new name, not a `create_func` overload, so no existing call can resolve differently |

The `Compiler` static flags (`cse_enabled_` etc.) stay as they are.

## 2. Which current IR types prevent doubles / CKKS constants?

| Location | Blocker |
|---|---|
| `src/fheco/dsl/common.hpp:9-11` | `using integer = std::int64_t; using PackedVal = std::vector<integer>;`, the value type for every constant and example value |
| `src/fheco/dsl/plaintext.hpp` | `Plaintext(integer scalar_val)`, `Plaintext(PackedVal)`: literals are integers only |
| `src/fheco/dsl/ciphertext.hpp` | example-value constructors take `PackedVal` / `integer` bounds |
| `src/fheco/ir/common.hpp` | `ConstInfo{bool is_scalar_; PackedVal val_;}`, `TermsValues`, `IOValues` all `PackedVal` |
| `src/fheco/ir/expr.cpp` | constant de-duplication keyed on `PackedVal` |
| `src/fheco/ir/func.cpp` (ctor) | builds `ClearDataEval` with `1 << plain_modulus_bit_size`; `plain_modulus_` is the only "parameter" a `Func` has |
| `src/fheco/util/clear_data_eval.*` | modular integer SIMD arithmetic (reduce mod `modulus_`) used for constant folding and example values |
| `src/fheco/trs/*` | constant matchers (`zero`, `one`, `m_one`) are `PackedVal` |
| `src/fheco/code_gen/gen_func.cpp` `gen_const_terms` | emits `vector<std::int64_t/std::uint64_t>` and `encoder.encode(vec, p)` (BatchEncoder form, no scale) |
| `benchmarks/utils.*` | `ClearArgInfo` is `variant<vector<int64_t>, vector<uint64_t>>`; parser uses `stoll/stoull`; outputs reduced mod the plain modulus; exact `==` comparison |
| out of scope | `egraphs/src/veclang.rs` `Num(i32)`; `RL/pytrs/parser.py` `int(token)` |

## 3. Can v1 avoid floating-point IR constants with named plaintext inputs?

**Yes, and Quad needs no constants or plaintexts at all.** `Plaintext("A")`
creates an input leaf whose value never enters the IR. Codegen emits
`Plaintext pN = encoded_inputs.at("A")` and the runtime supplies it. So
BatchNorm (`x*A + B`) and Chebyshev coefficients can later be expressed with no
IR change to `PackedVal`.

Caveats to resolve in milestone 2, not now:

- In CKKS a plaintext must be encoded at the level (`parms_id`) and scale of the
  ciphertext it meets. That is only known inside the generated `fhe()`, so CKKS
  plaintext inputs should reach `fhe()` as raw `vector<double>` and be encoded
  at the use site. The current signature passes pre-encoded `Plaintext`s.
- Integer literals such as the `2` and `-1` in the Chebyshev recurrence can stay
  in `PackedVal`; CKKS codegen would encode them as doubles.
- Example values / clear evaluation are integer-modular, so for CKKS v1 they
  must be rejected rather than silently wrong (see §10 guards).

## 4. Minimum new IR/opcode support for Quad

One opcode: **`OpCode::rescale`** (unary, cipher -> cipher, non-commutative,
`str_repr "rescale"`). Append `Type::rescale` at the **end** of
`OpCode::Type`, after `SumVec`, so the ordinal values of existing types (used in
`operator<` and hashing) do not move. No new `Term::Type`, no DSL operator (the
DSL never emits `rescale`), no `square`. `mul` and `relin` already exist.

Every exhaustive switch or lookup over opcode types must learn it. Several of
them **throw** on unknown ciphertext ops:

| File | Needed |
|---|---|
| `ir/op_code.hpp/.cpp` | declare/define `OpCode::rescale`; `operator<<` case |
| `ir/term.cpp` `deduce_result_type` | explicit "rescale arg must be cipher" check, like `relin` |
| `ir/common.cpp` `static_eval_op` | cost case (throws `unhandled op_code static cost` otherwise) |
| `util/quantifier.cpp` | op-count branch (throws `unhandled he operation` otherwise): `opposite_level_ + 1`, size unchanged; exclude `rescale` from `depth` in `compute_depth_info`, like `relin`/`mod_switch`; print a rescale count |
| `util/eval_on_clear.cpp` | pass-through case, like `relin` |
| `util/expr_printer.cpp` `ops_precedence_` | entry (precedence 1) |
| `passes/insert_relin.cpp` `get_ctxt_result_size` | pass-through case (used by `lazy_relin_heuristic`) |
| `code_gen/constants.hpp` `operation_mapping` | `{{rescale, {cipher}}, "rescale_to_next"}` |

TRS (`trs/term_op_code.*`) needs nothing: rescale is inserted after
`Compiler::compile`, so no ruleset ever sees it.

## 5. Where should rescale be represented?

**As an explicit IR term, inserted late, exactly like `relin`.** Not in the DSL,
not in the TRS, and not implicitly in codegen.

- It mirrors an existing, working pattern (`passes::relin_after_ctxt_ctxt_mul`
  inserts `relin` terms after `compile()`).
- `Expr::replace` already supports wrapping a term in a new op that uses it: it
  skips the new parent and moves output labels to it (`ir/expr.cpp:196`).
- Level consumption becomes visible to the Quantifier and to later analysis
  passes (level alignment, levels-consumed metric). An implicit codegen-only
  rescale would hide it.
- Inserting after `compile()` keeps the optimizer's view unchanged. Whether the
  LLM/TRS should later reason about rescale placement is a future decision.

## 6. Where should relinearization and rescaling be inserted?

In `Compiler::gen_he_code` (`dsl/compiler.cpp`), the existing post-compile hook:

```
reduce_rotation_keys              (unchanged)
relin_after_ctxt_ctxt_mul / lazy  (unchanged)
if (func->scheme() == Scheme::ckks) {
    passes::insert_rescale(func);      // new
    passes::check_ckks_levels(func);   // new
}
code_gen::gen_func(...)
```

New file `passes/insert_rescale.{hpp,cpp}`, registered in
`passes/CMakeLists.txt` and `passes/passes.hpp`:

- **`insert_rescale`**: for every cipher-result `mul`/`square` (ct-ct and ct-pt),
  wrap the term, or its `relin` if one was inserted, in `rescale` with
  `insert_op_term` + `replace_term_with`. The order is mul -> relin -> rescale,
  as in the oracle. This is the eager "rescale after every multiplication"
  policy the oracle uses.
- **`check_ckks_levels`**: walk terms in topological order and assign each
  cipher term a level (number of `rescale`/`mod_switch` on its path). Throw if a
  binary op mixes cipher operands at different levels. Throw if the deepest
  level exceeds the data levels in `CkksParams` (`coeff_mod_bit_sizes.size() - 2`).
  Expose the max level consumed for reporting.

## 7. Modulus-level alignment

- **v1 (Quad):** none needed, because the single multiply has one operand at
  one level. `check_ckks_levels` turns any mismatch into a clear compile-time
  error instead of a SEAL runtime exception.
- **v2 (BatchNorm/Chebyshev):** extend `check_ckks_levels` into an alignment
  pass. For a cipher-cipher binary op, insert the existing `OpCode::mod_switch`
  (already mapped to `mod_switch_to_next`) on the higher operand until levels
  match. For cipher-plain ops, codegen encodes the plaintext at the cipher's
  `parms_id`. Scales also need the oracle's exact-scale rule
  (encode at `out_scale * q_l / ct.scale()`), which needs `SEALContext` in
  `fhe()`. These are milestone-2 decisions and are deliberately left open.

## 8. Code generation changes

All CKKS output is guarded by `func->scheme() == Scheme::ckks`. BFV emission
paths stay byte-for-byte as they are.

| Item | Current code | CKKS change |
|---|---|---|
| `scheme_type::ckks` | `gen_func.cpp:375` writes `scheme_type::bfv` in `gen_main_code` | new `gen_main_code_ckks(const CkksParams&, SecurityLevel)` writes `EncryptionParameters params(scheme_type::ckks);`; no `set_plain_modulus` |
| `CKKSEncoder` | `constants.hpp:38` `encoder_type{"BatchEncoder"}` used by `gen_func_decl` / `gen_func_def_signature` | pass the encoder type into these two functions (or select it from the scheme). CKKS emits `const seal::CKKSEncoder &encoder`; SEAL 4.1's `CKKSEncoder::encode` overloads are `const`, so the const-ref signature works |
| Coefficient modulus chain | `gen_func.cpp:378-385`: `PlainModulus::Batching` + `BFVDefault` or the selector's sizes | `params.set_coeff_modulus(CoeffModulus::Create(n, {<coeff_mod_bit_sizes>}))` from `CkksParams` using the existing `gen_sequence` helper. `gen_func` must **skip** `ParameterSelector` / `EncParams(poly, plain_modulus)` for CKKS (`select_params` is BFV-only) |
| Scale | none | runtime: `const double scale = std::pow(2.0, <log2_scale>);` used to encode inputs. `fhe()` needs no scale for Quad (no constants); milestone 2 will |
| Rescale calls | none | from `operation_mapping`; the generic emitter in `gen_op_terms` produces `evaluator.rescale_to_next(cX, cY);` (same shape as `relinearize` minus the key) |
| Constants | `gen_const_terms` emits integer vectors | v1: throw `CKKS constants not supported yet` if a CKKS func has const terms |
| Galois keys | BFV main generates all power-of-two keys | CKKS main generates keys only for `get_rotation_steps_fhe()` (none for Quad; all keys at N = 32768 would be very large) |
| Security | `SEALContext(params, true, sec_level_type::tc128)` | same. N = 32768 with the oracle chain (520 bits) is within tc128 |

Expected generated `fhe()` for Quad:

```cpp
Ciphertext c1 = encrypted_inputs.at("x");
evaluator.multiply(c1, c1, c1);
evaluator.relinearize(c1, relin_keys, c1);
evaluator.rescale_to_next(c1, c1);
encrypted_outputs.emplace("y", move(c1));
```

**Parameters for milestone 1:** N = 32768, `{60, 40 x 10, 60}`, scale 2^40,
identical to the oracle. A smaller chain such as `{60, 40, 60}` would be faster,
but it is a parameter change and belongs in a separate controlled experiment.

## 9. Generated runtime harness

New files, leaving `benchmarks/utils.*` untouched:

- **`benchmarks/ckks_utils.hpp/.cpp`**:
  - Parse the same I/O layout as BFV (header `slot_count nb_inputs nb_outputs`;
    input line `label is_cipher is_signed v...`; output line `label is_cipher v...`)
    into `unordered_map<string, vector<double>>`; `is_signed` is ignored.
  - Encode with `CKKSEncoder` at the scale and encrypt. Inputs are replicated
    cyclically to fill all slots, as the BFV harness does.
  - Decrypt, decode, and resize outputs to `func_slot_count`.
  - Compare with a **tolerance**: max absolute error per output, PASS/FAIL,
    default 1e-4 (same as the oracle), overridable with `--tol`.
  - Report each output's chain index and log2 scale.
- **`gen_main_code_ckks`**: same structure as the BFV template (`../fhe_io_example_adapted.txt`, `_gen_he_fhe.hpp`, `fhe(...)`), plus:
  - prints `execution_time_(ms): <t>` using **CHEHAB's** definition (parse +
    encode + encrypt + `fhe()`, single run), so `run_benchmarks.py` parsing
    still works;
  - also prints `fhe_eval_median_(ms): <t>` (1 warmup + N runs of `fhe()` only),
    comparable with the oracle;
  - exits non-zero on FAIL. The BFV generated main performs no correctness
    check at all; that stays as is.
- **`benchmarks/quad_ckks/`**:
  - `quad_ckks.cpp`: the DSL program above via `create_ckks_func("fhe", slot_count, params)`,
    then `Compiler::compile(simplification_ruleset)`, `gen_he_code`, and the
    Quantifier (scalar path only; vectorizer never called);
  - `generate_quad_ckks.py`: random doubles in [-2, 2] and an independent numpy `x**2` reference;
  - `CMakeLists.txt` modelled on `poly_reg`, copying `ckks_utils.*`;
  - `he/CMakeLists.txt` linking `ckks_utils`.
- **Root `CMakeLists.txt`**: `option(FHECO_BUILD_CKKS_BENCHMARKS ... OFF)` guarding
  `add_subdirectory(benchmarks/quad_ckks)`, so the default build is unchanged.

## 10. BFV behavior that must remain unchanged

- Existing `create_func` overloads, `Func` constructors and their defaults (BFV).
- Generated `_gen_he_fhe.hpp/.cpp` and `he/main.cpp` for every BFV benchmark:
  **byte-identical** (golden check below).
- Quantifier output for BFV programs (rescale branches are never hit).
- `relin_after_ctxt_ctxt_mul`, `lazy_relin_heuristic`, `reduce_rotation_keys`,
  `prepare_code_gen`, `ParameterSelector`, all TRS rulesets.
- `benchmarks/utils.*`, every existing benchmark directory, `run_benchmarks.py`.
- `egraphs/`, `RL/`, vectorization (`gen_vectorized_code`, `format_vectorized_code`).
- Ordinal values of existing `OpCode::Type` entries (append-only).

**CKKS v1 guards.** These throw clear errors rather than computing wrong values:

- example values on CKKS inputs
- constant terms in a CKKS func
- `gen_vectorized_code` on a CKKS func
- `lazy_relin` + CKKS (until it is tested)

## Implementation sequence (smallest path to Quad)

Each step is one commit and ends with a gate. The BFV golden check runs at
every step from step 1 on.

0. **BFV golden snapshot** (test-only commit). A script that builds `fheco`,
   runs `poly_reg` (scalar path, `0 4 0 0 1 1 1`) and the BFV Quad probe from
   §0, and stores the generated `he/_gen_he_fhe.{hpp,cpp}`, `he/main.cpp` and
   Quantifier output. *Gate:* the snapshot is reproducible run-to-run.
1. **Scheme plumbing**: `Scheme`/`CkksParams` (dsl/common.hpp), `Func` fields and
   CKKS constructor, `Compiler::create_ckks_func`, v1 guards. *Gate:* BFV golden
   identical; a CKKS func builds the IR for `x * x` (Quantifier `(1, 1)`).
2. **`OpCode::rescale`** in every site of the §4 table. *Gate:* BFV golden identical.
3. **Passes**: `insert_rescale`, `check_ckks_levels`, wired into `gen_he_code`
   for CKKS only. *Gate:* IR is `mul -> relin -> rescale`; Quantifier reports
   one level consumed; BFV golden identical.
4. **CKKS codegen**: encoder type, `rescale_to_next` mapping, selector bypass,
   `gen_main_code_ckks`, constant guard. *Gate:* generated `fhe()` equals the
   §8 listing; generated sources compile against SEAL 4.1; BFV golden identical.
5. **CKKS runtime**: `benchmarks/ckks_utils.*`. *Gate:* unit check that
   encode -> decode round-trips an I/O file within 1e-9.
6. **`benchmarks/quad_ckks`** + root CMake option. *Gate (milestone):*
   compile phase -> `he/` build -> `./main` prints PASS. Max error ~1e-8, chain
   10 -> 9, output scale ~2^40.000002 (all matching the oracle's Quad row).
   Record CHEHAB compile time, `execution_time_(ms)` and `fhe_eval_median_(ms)`
   next to the oracle's 36 ms median.

## Reusing CKKS code from older CHEHAB branches

The pre-rewrite compiler (`fheco/` tree; a `Program`/DAG/`Translator`
architecture, not today's `Func`/`Expr`/`code_gen`) is the only CKKS-era code.
**Nothing is portable verbatim; three small ideas are worth reusing.** Evidence
that its CKKS path never ran end-to-end:

- `OpCode::rescale` is declared and mapped but no `.cpp` on any branch inserts it.
- It maps rescale to the SEAL method name `"rescale"`, which does not exist
  (SEAL has `rescale_to_next`).
- Constants stay `int64`/`uint64` even for CKKS.
- Only BFV examples exist (`examples/bfv`, `examples/seal_bfv`).

| Old location (branch @ commit) | What it has | Use |
|---|---|---|
| `fheco/src/utils/fhecompiler_const.hpp` (`add-benchmarks` @ `84a66cc`, 2023-04-16) | `enum Scheme { none, bfv, bgv, ckks }` | idea only; §1 uses a scoped 2-value enum |
| `fheco/src/translator/translator_const.hpp`, `ContextWriter::write_parameters` (same commit) | scheme string from enum; **skips `set_plain_modulus` for CKKS**; `CoeffModulus::Create(n, {...})` emission | reuse the logic in `gen_main_code_ckks` |
| same file, `ckks_encoder_type_literal`, `ops_map` | `seal::CKKSEncoder`; rescale -> `"rescale"` (wrong name) | encoder literal only |
| `fheco/fhecompiler.cpp` `init(..., Scheme, double scale)` (`parameter-selection-and-maintenance-scheduling` @ `f954640`, 2023-03-18) | throws `"scale is missing for CKKS"` | reuse the validation in `create_ckks_func` |
| `fheco/src/params_selector/params_selector.cpp` (`add-benchmarks` @ `84a66cc`) | CKKS slot count = N/2 check | reuse: `slot_count <= poly_modulus_degree / 2` |
| `fheco/src/translator/translator_const.hpp` `EncodingWriter` | encodes `vector<int64>` with a scale | do **not** port (integer CKKS constants) |
| `ufhe/src/ufhe/api/evaluator.hpp` (on `main`) | abstract `rescale_to_next_inplace`; CKKS marked TODO | not built by root CMake, not used by codegen; ignore |

Older `dev-ir` @ `a0afa9a` (2022-12-27) and `develop` @ `0303668` (2022-11-07)
only add the same enum checks and the `ufhe` stubs.

## Open decisions (not needed for Quad)

1. Plaintext-input encoding for CKKS: raw doubles into `fhe()`, encoded at the
   use site (recommended), versus pre-encoded at a declared level.
2. Scale management in generated code: port the oracle's exact-scale rule,
   which needs `SEALContext` passed to `fhe()`.
3. Whether rescale/mod_switch placement should ever be visible to the TRS/LLM
   optimizer, which would change the depth/level trade-offs it can explore.
