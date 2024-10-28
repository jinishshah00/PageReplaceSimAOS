README  
------

**Project:** Page Replacement Algorithms Simulation

**Folder Structure:**

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

### Instructions

1. **Compilation:**  
   Navigate to the `/PageReplacementAlgo` directory and execute:

   ```bash
   make
   ```

   This will compile the program and produce an executable named `paging`.

2. **Execution:**  
   Run the executable using:

   ```bash
   ./paging [Algorithm] [RandomSeed]
   ```

   - **Algorithm Options:** `FIFO`, `LRU`, `LFU`, `MFU`, `Random`
   - **Random Seed:** Use any integer or `RAND` for a random seed.

   **Example Command:**

   ```bash
   ./paging FIFO 12345
   ```

3. **Output:**  
   - Each algorithm’s output is saved in the `/outputs/` folder.
   - Each output file contains logs for five runs, capturing details of page references, hits/misses, and evictions.

### File Descriptions

- **main.c:** Handles program initialization and main simulation logic.
- **constants.h:** Defines global constants for the simulation.
- **globals.c:** Manages shared variables and memory structures.

### Note

Ensure all necessary source files are in the `/src/` directory before compiling.