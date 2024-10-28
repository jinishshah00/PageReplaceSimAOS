```markdown
# Page Replacement Algorithms Simulation

## Project Structure
```
/PageReplacementAlgo/
  ├── /src/
  │   ├── main.c
  │   ├── constants.h
  │   ├── globals.c
  │   └── (other source files)
  ├── /outputs/
  │   └── output.txt (for each algorithm, repeated 5 times)
  ├── Makefile
  ├── .gitignore
  └── README.md
```

## Instructions

1. **Compilation:**
   - Navigate to the `/PageReplacementAlgo` directory and run:
     ```bash
     make
     ```
   - This will compile the project and generate an executable named `paging`.

2. **Execution:**
   - Run the executable using:
     ```bash
     ./paging [Algorithm] [RandomSeed]
     ```
     - **Algorithm Options:** `FIFO`, `LRU`, `LFU`, `MFU`, `Random`
     - **Random Seed:** Provide an integer or use `RAND` for a random seed.

   - **Example Command:**
     ```bash
     ./paging FIFO 12345
     ```

3. **Output:**
   - Results for each algorithm run are saved in the `/outputs/` directory.
   - Each output file contains logs for five runs, showing page references, hit/miss ratios, and page evictions.


## Note
Ensure all source files are in the `/src/` directory before compiling.
```