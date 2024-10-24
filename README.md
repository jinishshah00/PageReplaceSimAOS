# Page Replacement Algorithm Simulator - Input Parameters

This simulator allows you to experiment with various page replacement algorithms using input parameters at runtime.

## Usage

```bash
./simulator [ALGORITHM] [RANDOM_SEED] [STEP_OPTION]
```

### Input Parameters:

1. **Algorithm Selection**:
   - Choose from the following algorithms:
     - `FCFS` (First-Come-First-Serve)
     - `LRU` (Least Recently Used)
     - `LFU` (Least Frequently Used)
     - `MFU` (Most Frequently Used)
     - `Random` (Random Page Replacement)

   Example:
   ```bash
   ./simulator LRU
   ```

2. **Random Seed**:
   - Use `RAND` for a random seed based on system time or provide a specific integer for reproducibility.

   Example:
   ```bash
   ./simulator FCFS 1234
   ```

3. **Step Option** (Optional):
   - Use `1` to enable step mode after 100 page accesses.

### Example Commands:

```bash
./simulator Random RAND
./simulator MFU 1234 1
```
