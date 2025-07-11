# Syzygy_Zopfli
C to Rust Translation of Zopfli


## Build

1. Rust: `cargo build --release`

2. C: build the test binary using both `zopfli.c` and `zopfli_tests.c`:
   `gcc zopfli.c zopfli_tests.c -o zopfli -lm` (run inside `c_code`)
   The resulting `zopfli` binary runs a small suite of built-in tests.


## Test

Create `NUM_TESTS` tests in the `TEST_DIR` (see `run_equiv.py`). Run as `python run_equiv.py`. Dependencies are in `requirements.txt`
