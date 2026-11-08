# Fair-parameters proxy: every kernel (baseline included) first drops its unused levels
# (x * one, k = 10 - levels times), so all of them finish at the bottom of the modulus chain.
import sys, json, re, shutil, tempfile
from pathlib import Path
sys.path.insert(0, '/Users/abyan/Documents/Senior/Capstone/fhe-agentic-optimizer/agent')
from evaluate import evaluate, snapshot
from loop import interleaved
from selection import FINAL_BATCHES, confirm
from tasks import TASKS
here = Path(__file__).parent
seal = '/Users/abyan/Documents/Senior/Capstone/deps/seal-install'
TOTAL = 10

def floored(src, k):
    src = re.sub(r'\s*Plaintext one\("one"\);', '', src).replace(', one("one")', '')
    chain = '\n  Plaintext one("one");\n  Ciphertext xb0 = x;'
    for i in range(k):
        chain += f'\n  Ciphertext xb{i+1} = xb{i} * one;'
    src = src.replace('Ciphertext x("x");', 'Ciphertext x("x");' + chain, 1)
    return src.replace('Ciphertext z = x * alpha;', f'Ciphertext z = xb{k} * alpha;', 1)

tmp = Path(tempfile.mkdtemp(prefix='floor-'))
out = []
for t in ['chebyshev_elu_d7', 'chebyshev_selu_d7', 'chebyshev_mish_d7']:
    task = TASKS[t]
    srcs = {'baseline': task.baseline(), **{k: (here / f'{k}.cpp').read_text() for k in ['clenshaw', 'shallow', 'combo_a', 'combo_b']}}
    for k, src in srcs.items():
        lv = evaluate(task, src, seal, 1).levels
        fsrc = floored(src, TOTAL - lv)
        (here / f'floor_{t}_{k}.cpp').write_text(fsrc)
        ev = evaluate(task, fsrc, seal, 10)
        row = {'task': t, 'kernel': k, 'own_levels': lv, 'burned': TOTAL - lv, 'status': ev.status, 'levels': ev.levels,
               'ops': ev.ops, 'max_err': ev.max_abs_error, 'msg': '' if ev.correct else ev.message[:600]}
        if ev.correct:
            snapshot(task, tmp / t / k)
            if k != 'baseline':
                c = confirm(*interleaved(task, tmp / t / k, tmp / t / 'baseline', FINAL_BATCHES, 10))
                row.update(speedup_vs_floored_baseline=round(c['speedup'], 3), ms=round(c['candidate_ms'], 1),
                           base_ms=round(c['reference_ms'], 1), wins=c['wins'])
        print(json.dumps(row), flush=True); out.append(row)
shutil.rmtree(tmp)
(here / 'results_floor.json').write_text(json.dumps(out, indent=1))
