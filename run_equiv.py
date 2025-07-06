import tqdm
import subprocess

N_TESTS = 1000000
N_TESTS_STA = 0
N_TESTS_END = 1000000

# Tests are in here named as: test_<idx=0> 
TEST_DIR = "/dev/test_folder"

# Number of worker threads to spawn
NUM_WORKERS=180

def run_single_test(test_idx: int):
    command_1 = f"./c_code/zopfli  < {TEST_DIR}/test_{test_idx}"
    command_2 = f"./rust_code/target/release/testing_equiv < {TEST_DIR}/test_{test_idx}"

    o1 = subprocess.run(
        command_1, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )
    o2 = subprocess.run(
        command_2, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
    )

    # get stdout stderr combined
    ret1 = o1.stderr.decode()
    ret2 = o2.stderr.decode()

    assert ret1 == ret2, f"Test {test_idx} failed {ret1}"
    return ret1, ret2


def run_all_tests_serial():
    for test_idx in tqdm.tqdm(range(N_TESTS)):
        run_single_test(test_idx)


## run parallel
from custom_multi import run_tasks_in_parallel, TaskRunStatus


def run_all_tests_parallel():
    tasks = list(range(N_TESTS_STA, N_TESTS_END))
    results = run_tasks_in_parallel(
        run_single_test, tasks, num_workers=NUM_WORKERS, use_progress_bar=True
    )
    outputs = []
    for idx, r in enumerate(results):
        # print(r.exception_tb)
        # print(r.status)
        if r.status != TaskRunStatus.SUCCESS:
            print(r.exception_tb)
            print(r.result)
            print("#### ", idx, " failed")
            continue
        result = r.result
        assert result[0] == result[1], f"Test {r.task_id} failed"
        outputs.append(result)
    return outputs


if __name__ == "__main__":
    outputs = run_all_tests_parallel()
