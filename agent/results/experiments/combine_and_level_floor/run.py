import sys, json, shutil, tempfile
from pathlib import Path
sys.path.insert(0, '/Users/abyan/Documents/Senior/Capstone/fhe-agentic-optimizer/agent')
from evaluate import evaluate, snapshot
from loop import interleaved
from selection import FINAL_BATCHES, confirm
from tasks import TASKS
here = Path(__file__).parent
seal = '/Users/abyan/Documents/Senior/Capstone/deps/seal-install'
kernels = ['clenshaw', 'shallow', 'combo_a', 'combo_b', 'clenshaw_burn2']
tmp = Path(tempfile.mkdtemp(prefix='combine-'))
out = []
for t in ['chebyshev_elu_d7', 'chebyshev_selu_d7', 'chebyshev_mish_d7']:
    task = TASKS[t]
    b = evaluate(task, task.baseline(), seal, 10); snapshot(task, tmp / t / 'baseline')
    for k in kernels:
        ev = evaluate(task, (here / f'{k}.cpp').read_text(), seal, 10)
        row = {'task': t, 'kernel': k, 'status': ev.status, 'levels': ev.levels, 'depth': (ev.depth, ev.mult_depth), 'ops': ev.ops,
               'max_err': ev.max_abs_error, 'msg': '' if ev.correct else ev.message[:900]}
        if ev.correct:
            snapshot(task, tmp / t / k)
            c = confirm(*interleaved(task, tmp / t / k, tmp / t / 'baseline', FINAL_BATCHES, 10))
            row.update(speedup=round(c['speedup'], 3), ms=round(c['candidate_ms'], 1), base_ms=round(c['reference_ms'], 1), wins=c['wins'])
        print(json.dumps(row), flush=True); out.append(row)
shutil.rmtree(tmp)
(here / 'results.json').write_text(json.dumps(out, indent=1))
