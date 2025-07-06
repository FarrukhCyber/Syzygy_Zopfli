# Syzygy_Zopfli
C to Rust Translation of Zopfli


## Build

1. Rust: `cargo build --release`

2. C: use `gcc` and build to output `zopfli` (in `c_code`)


## Test

Create `NUM_TESTS` tests in the `TEST_DIR` (see `run_equiv.py`). Run as `python run_equiv.py`. Dependencies are in `requirements.txt`