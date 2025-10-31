TODO: Junzhe Shi & Albert Wang  
MS1: Read the project and get familiar with MS1-3 and the main purpose of this assignment.  

MS1:
Junzhe: input read, input parse, hardcoded stdout  
Albert: input validation  

MS2:
Junzhe: LRU basic setup and load implementation for LRU  
Albert: store implementation for LRU  

TODO (for MS3): best cache report
Junzhe: Design experiments, run and collect data, and compose the report
Albert: MS3 FIFO Load, data collection, and Final check and validation.  

# Cache Simulator Project Report

## Project Overview

This cache simulator project uses nested vector to imitate the cache working process and gain the data of total cycles, total hit and misses, and the derived hit rate and miss rate. Data is used to compare the overall efficiency via different set of experiments.  

## Methodology

We tested cache performance by isolating one factor at a time—capacity, block size, associativity, replacement policy, and write policy—while keeping other settings constant. Each group of experiments used the csim simulator on benchmark trace, gcc.trace. We measured hit and miss counts, total cycles, and hit rates to see how each factor affected efficiency. A final fixed-size test verified that improvements came from structure, not just larger capacity. Based on analyzation above, we will come to a conclusion at best overall perfomance of cache.  

NOTICE: we use the mostly accepted efficiency calculation called Average Acessed Time (AAT) = Hit Time + Miss Rate * Miss Penalty  

## Experiments

### Group 1: Cache Size
| ID | Sets | Assoc | Block | Write | Alloc | Repl | Size |
|----|------|--------|--------|--------|--------|--------|----------|
| **C1** | 64 | 1 | 32 | write-back | write-allocate | lru | 2 KB |
| **C2** | 256 | 1 | 32 | write-back | write-allocate | lru | 8 KB |
| **C3** | 512 | 1 | 32 | write-back | write-allocate | lru | 16 KB |
| **C4** | 1024 | 1 | 32 | write-back | write-allocate | lru | 32 KB |
| **C5** | 2048 | 1 | 32 | write-back | write-allocate | lru | 64 KB |
| **C6** | 4096 | 1 | 32 | write-back | write-allocate | lru | 128 KB |
| **C7** | 8192 | 1 | 32 | write-back | write-allocate | lru | 256 KB |
| **C8** | 16384 | 1 | 32 | write-back | write-allocate | lru | 512 KB |
| **C9** | 32768 | 1 | 32 | write-back | write-allocate | lru | 1024 KB |

**results**:
gcc.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| C1 | 318197      | 197486       | 279340    | 38857       | 183946     | 13540        | 58834883     |
| C2 | 318197      | 197486       | 307356    | 10841       | 190546     | 6940         | 22211683     |
| C3 | 318197      | 197486       | 312715    | 5482        | 191972     | 5514         | 14472483     |
| C4 | 318197      | 197486       | 314254    | 3943        | 192479     | 5007         | 11865283     |
| C5 | 318197      | 197486       | 315381    | 2816        | 192775     | 4711         | 9462883      |
| C6 | 318197      | 197486       | 315938    | 2259        | 192838     | 4648         | 7678083      |
| C7 | 318197      | 197486       | 316106    | 2091        | 192907     | 4579         | 6392483      |
| C8 | 318197      | 197486       | 316640    | 1557        | 192931     | 4555         | 5593283      |
| C9 | 318197      | 197486       | 316654    | 1543        | 192938     | 4548         | 5462883      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) | AAT Improvement |
|----|------------------|-------------------|----------------|--------------|-----------------|
| C1 | 89.84%           | 10.16%            | 515683         | 114.08       | -               |
| C2 | 96.55%           | 3.45%             | 515683         | 43.08        | 62.24%          |
| C3 | 97.86%           | 2.14%             | 515683         | 28.06        | 34.85%          |
| C4 | 98.26%           | 1.74%             | 515683         | 23.01        | 18.00%          |
| C5 | 98.54%           | 1.46%             | 515683         | 18.35        | 20.25%          |
| C6 | 98.65%           | 1.35%             | 515683         | 14.89        | 18.86%          |
| C7 | 98.70%           | 1.30%             | 515683         | 12.40        | 16.72%          |
| C8 | 98.81%           | 1.19%             | 515683         | 10.85        | 12.50%          |
| C9 | 98.82%           | 1.18%             | 515683         | 10.59        | 2.40%           |

swim.trace:

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) | AAT Improvement |
|----|------------------|-------------------|----------------|--------------|-----------------|
| C1 | 86.66%           | 13.34%            | 303193         | 140.46       | -               |
| C2 | 96.51%           | 3.49%             | 303193         | 48.86        | 65.22%          |
| C3 | 97.11%           | 2.89%             | 303193         | 41.15        | 15.78%          |
| C4 | 97.51%           | 2.49%             | 303193         | 36.30        | 11.78%          |
| C5 | 97.75%           | 2.25%             | 303193         | 30.02        | 17.30%          |
| C6 | 97.85%           | 2.15%             | 303193         | 24.53        | 18.28%          |
| C7 | 97.86%           | 2.14%             | 303193         | 20.26        | 17.40%          |
| C8 | 97.86%           | 2.14%             | 303193         | 19.08        | 5.83%           |
| C9 | 97.89%           | 2.11%             | 303193         | 17.98        | 5.76%           |

**Analysis**:
Param sets above evaluate the key factors listed affected by the change in cache size with all other factors fixed. We expect the miss rate to be as low as possible (or hit rate as high as possible) since this will increase the efficiency and reduce the access to memory, fully utilizing the cache.  

Looking at the miss rates and total cycles, we can see that C1-C7, there is a very obvious decreasing rate in missing rates and total cycles, increase size gives us better performance. However, from C7-C9, we can only see diminishing return in improvement rate, and also due to cost-efficiency trade-off, we tend to keep the cache size lower so it will behave different from memory more.  

Therefore, the best size determined is 256KB for now.  

## Group 2: Block Size Analysis
| ID | Sets | Assoc | Block | Write | Alloc | Repl | Size |
|----|------|--------|--------|--------|--------|--------|----------|
| **B1** | 16384 | 1 | 16 | write-back | write-allocate | lru | ≈256 KB |
| **B2** | 8192 | 1 | 32 | write-back | write-allocate | lru | ≈256 KB |
| **B3** | 4096 | 1 | 64 | write-back | write-allocate | lru | ≈256 KB |
| **B4** | 2048 | 1 | 128 | write-back | write-allocate | lru | ≈256 KB |
| **B5** | 1024 | 1 | 256 | write-back | write-allocate | lru | ≈256 KB |
| **B6** | 512 | 1 | 512 | write-back | write-allocate | lru | ≈256 KB |
| **B7** | 256 | 1 | 1024 | write-back | write-allocate | lru | ≈256 KB |

**results**:
gcc.trace:
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| B1 | 318197      | 197486       | 315399    | 2798        | 188575     | 8911         | 5540083      |
| B2 | 318197      | 197486       | 316106    | 2091        | 192907     | 4579         | 6392483      |
| B3 | 318197      | 197486       | 316465    | 1732        | 195043     | 2443         | 8128483      |
| B4 | 318197      | 197486       | 316442    | 1755        | 196129     | 1357         | 12109283     |
| B5 | 318197      | 197486       | 316528    | 1669        | 196678     | 808          | 19286883     |
| B6 | 318197      | 197486       | 315706    | 2491        | 196862     | 624          | 47875683     |
| B7 | 318197      | 197486       | 315352    | 2845        | 197031     | 455          | 100202083    |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) | AAT Improvement |
|----|------------------|-------------------|----------------|--------------|-----------------|
| B1 | 97.75%           | 2.25%             | 515683         | 10.74        | -               |
| B2 | 98.70%           | 1.30%             | 515683         | 12.40        | -15.46%         |
| B3 | 99.19%           | 0.81%             | 515683         | 15.76        | -27.10%         |
| B4 | 99.40%           | 0.60%             | 515683         | 23.49        | -49.07%         |
| B5 | 99.52%           | 0.48%             | 515683         | 37.40        | -59.22%         |
| B6 | 99.39%           | 0.61%             | 515683         | 92.84        | -148.34%        |
| B7 | 99.36%           | 0.64%             | 515683         | 194.33       | -109.28%        |

swim.trace:
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| B1 | 220668      | 82525        | 219646    | 1022        | 72019      | 10506        | 5427993      |
| B2 | 220668      | 82525        | 219987    | 681         | 76698      | 5827         | 6143193      |
| B3 | 220668      | 82525        | 220141    | 527         | 79399      | 3126         | 7045593      |
| B4 | 220668      | 82525        | 220202    | 466         | 80850      | 1675         | 8645593      |
| B5 | 220668      | 82525        | 220233    | 435         | 81576      | 949          | 11855193     |
| B6 | 220668      | 82525        | 220225    | 443         | 81903      | 622          | 19336793     |
| B7 | 220668      | 82525        | 220104    | 564         | 82002      | 523          | 41851993     |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) | AAT Improvement |
|----|------------------|-------------------|----------------|--------------|-----------------|
| B1 | 96.22%           | 3.78%             | 303193         | 17.90        | -               |
| B2 | 97.86%           | 2.14%             | 303193         | 20.26        | -13.19%         |
| B3 | 98.80%           | 1.20%             | 303193         | 23.23        | -14.66%         |
| B4 | 99.29%           | 0.71%             | 303193         | 28.52        | -22.78%         |
| B5 | 99.54%           | 0.46%             | 303193         | 39.11        | -37.13%         |
| B6 | 99.65%           | 0.35%             | 303193         | 63.78        | -63.10%         |
| B7 | 99.64%           | 0.36%             | 303193         | 138.04       | -116.46%        |

**Analysis**:
The trend apparently shows that smaller block size gives the better AAT. However, this does not mean we can just take the smaller block, because this will increase the risk of conclict misses and fragmentation.  

In addition, we can take a look at the overall hit and miss rate, they increases and decreases as the block size increasing. The increase and decrease become marginal after B2-B3, the number is kept at 99% level. The changes after B4 for hit rate and miss rate is marginal and continue increasing block size will worse AAT, thus we select the optimzal size of block to be 32 to 64B which also match the real-world computer caches.  

## Group 3.1: Associativity Analysis
| ID | Sets | Assoc | Block | Write | Alloc | Repl | Size |
|----|------|--------|--------|--------|--------|--------|----------|
| **A1** | 8192 | 1 | 32 | write-back | write-allocate | lru | 256 KB |
| **A2** | 4096 | 2 | 32 | write-back | write-allocate | lru | 256 KB |
| **A3** | 2048 | 4 | 32 | write-back | write-allocate | lru | 256 KB |
| **A4** | 1024 | 8 | 32 | write-back | write-allocate | lru | 256 KB |
| **A5** | 512 | 16 | 32 | write-back | write-allocate | lru | 256 KB |
| **A6** | 256 | 32 | 32 | write-back | write-allocate | lru | 256 KB |
| **A7** | 128 | 64 | 32 | write-back | write-allocate | lru | 256 KB |

**results:**
gcc.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| A1 | 318197      | 197486       | 316106    | 2091        | 192907     | 4579         | 6392483      |
| A2 | 318197      | 197486       | 316662    | 1535        | 192942     | 4544         | 5669283      |
| A3 | 318197      | 197486       | 316676    | 1521        | 192948     | 4538         | 5554083      |
| A4 | 318197      | 197486       | 316691    | 1506        | 192949     | 4537         | 5438083      |
| A5 | 318197      | 197486       | 316690    | 1507        | 192952     | 4534         | 5394883      |
| A6 | 318197      | 197486       | 316691    | 1506        | 192954     | 4532         | 5378083      |
| A7 | 318197      | 197486       | 316692    | 1505        | 192955     | 4531         | 5354083      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) | AAT Improvement |
|----|------------------|-------------------|----------------|--------------|-----------------|
| A1 | 98.70%           | 1.30%             | 515683         | 12.40        | -               |
| A2 | 98.82%           | 1.18%             | 515683         | 10.99        | 11.37%          |
| A3 | 98.83%           | 1.17%             | 515683         | 10.77        | 2.00%           |
| A4 | 98.84%           | 1.16%             | 515683         | 10.54        | 2.14%           |
| A5 | 98.84%           | 1.16%             | 515683         | 10.46        | 0.76%           |
| A6 | 98.84%           | 1.16%             | 515683         | 10.43        | 0.29%           |
| A7 | 98.84%           | 1.16%             | 515683         | 10.38        | 0.48%           |

swim.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| A1 | 220668      | 82525        | 219987    | 681         | 76698      | 5827         | 6143193      |
| A2 | 220668      | 82525        | 220067    | 601         | 76719      | 5806         | 5801593      |
| A3 | 220668      | 82525        | 220076    | 592         | 76720      | 5805         | 5486393      |
| A4 | 220668      | 82525        | 220077    | 591         | 76720      | 5805         | 5435193      |
| A5 | 220668      | 82525        | 220077    | 591         | 76720      | 5805         | 5423193      |
| A6 | 220668      | 82525        | 220077    | 591         | 76720      | 5805         | 5420793      |
| A7 | 220668      | 82525        | 220077    | 591         | 76720      | 5805         | 5419993      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) | AAT Improvement |
|----|------------------|-------------------|----------------|--------------|-----------------|
| A1 | 97.86%           | 2.14%             | 303193         | 20.26        | -               |
| A2 | 97.89%           | 2.11%             | 303193         | 19.13        | 5.58%           |
| A3 | 97.90%           | 2.10%             | 303193         | 18.09        | 5.44%           |
| A4 | 97.90%           | 2.10%             | 303193         | 17.92        | 0.94%           |
| A5 | 97.90%           | 2.10%             | 303193         | 17.88        | 0.22%           |
| A6 | 97.90%           | 2.10%             | 303193         | 17.88        | 0.00%           |
| A7 | 97.90%           | 2.10%             | 303193         | 17.87        | 0.06%           |

**Analysis**:
The trend that higher associativity gives better AAT up to a certain point.  However, this does not mean we can just take the highest associativity, because this will increase hardware complexity and access time overhead with a marginal gainning in AAT.  

In addition, we can take a look at the overall hit and miss rate, they increase and decrease as the associativity increasing. The increase and decrease become marginal after A3, the number is kept at 98.84% level for gcc and 97.90% for swim. The changes after A4 for hit rate and miss rate is marginal and continue increasing associativity will have diminishing returns on AAT improvement, thus we select the optimal associativity to be 4-way which also match the real-world computer caches.  

##Group 3.2: Select between 32 and 64 Block Size for 4-way Associativity
| ID | Sets | Assoc | Block | Write | Alloc | Repl | Size |
|----|------|--------|--------|--------|--------|--------|----------|
| **A3** | 2048 | 4 | 32 | write-back | write-allocate | lru | 256 KB |
| **EX** | 1024 | 4 | 64 | write-back | write-allocate | lru | 256 KB |

**results:**
gcc.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| A3 | 318197      | 197486       | 316676    | 1521        | 192948     | 4538         | 5554083      |
| EX | 318197      | 197486       | 317189    | 1008        | 195106     | 2380         | 6254883      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) |
|----|------------------|-------------------|----------------|--------------|
| A3 | 98.83%           | 1.17%             | 515683         | 10.77        |
| EX | 99.34%           | 0.66%             | 515683         | 12.13        |

swim.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| A3 | 220668      | 82525        | 220076    | 592         | 76720      | 5805         | 5486393      |
| EX | 220668      | 82525        | 220274    | 394         | 79464      | 3061         | 5978393      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) |
|----|------------------|-------------------|----------------|--------------|
| A3 | 97.90%           | 2.10%             | 303193         | 18.09        |
| EX | 98.86%           | 1.14%             | 303193         | 19.72        |

Directly compare, we get 32 for block size the best in this simulator.

## Group4: Write and Replacement Policy
| ID | Sets | Assoc | Block | Write | Alloc | Repl | Size |
|----|------|--------|--------|--------|--------|--------|----------|
| **W1** | 2048 | 4 | 32 | write-through | write-allocate | lru | 256 KB |
| **W2** | 2048 | 4 | 32 | write-back | write-allocate | lru | 256 KB |
| **W3** | 2048 | 4 | 32 | write-through | no-write-allocate | lru | 256 KB |
| **R1** | 2048 | 4 | 32 | write-back | write-allocate | lru | 256 KB |
| **R2** | 2048 | 4 | 32 | write-back | write-allocate | fifo | 256 KB |

**results:**
gcc.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| W1 | 318197      | 197486       | 316676    | 1521        | 192948     | 4538         | 25111483     |
| W2 | 318197      | 197486       | 316676    | 1521        | 192948     | 4538         | 5554083      |
| W3 | 318197      | 197486       | 314402    | 3795        | 169733     | 27753        | 23272530     |
| R1 | 318197      | 197486       | 316676    | 1521        | 192948     | 4538         | 5554083      |
| R2 | 318197      | 197486       | 316647    | 1550        | 192944     | 4542         | 5592483      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) |
|----|------------------|-------------------|----------------|--------------|
| W1 | 98.83%           | 1.17%             | 515683         | 48.69        |
| W2 | 98.83%           | 1.17%             | 515683         | 10.77        |
| W3 | 93.88%           | 6.12%             | 515683         | 45.11        |
| R1 | 98.83%           | 1.17%             | 515683         | 10.77        |
| R2 | 98.82%           | 1.18%             | 515683         | 10.84        |

swim.trace
| ID | Total Loads | Total Stores | Load Hits | Load Misses | Store Hits | Store Misses | Total Cycles |
|----|-------------|--------------|-----------|-------------|------------|--------------|--------------|
| W1 | 220668      | 82525        | 220076    | 592         | 76720      | 5805         | 13673293     |
| W2 | 220668      | 82525        | 220076    | 592         | 76720      | 5805         | 5486393      |
| W3 | 220668      | 82525        | 218642    | 2026        | 62147      | 20378        | 10156115     |
| R1 | 220668      | 82525        | 220076    | 592         | 76720      | 5805         | 5486393      |
| R2 | 220668      | 82525        | 220074    | 594         | 76720      | 5805         | 5486393      |

| ID | Overall Hit Rate | Overall Miss Rate | Total Accesses | AAT (cycles) |
|----|------------------|-------------------|----------------|--------------|
| W1 | 97.90%           | 2.10%             | 303193         | 45.11        |
| W2 | 97.90%           | 2.10%             | 303193         | 18.09        |
| W3 | 92.59%           | 7.41%             | 303193         | 33.50        |
| R1 | 97.90%           | 2.10%             | 303193         | 18.09        |
| R2 | 97.90%           | 2.10%             | 303193         | 18.09        |

**Analysis**:
The trend shows that write policy has significant impact on performance. Write-back (W2) performs much better than write-through (W1) due to reduced memory traffic. No-write-allocate (W3) shows lower hit rates and worse performance compared to write-allocate configurations.  
In addition, replacement policy comparison shows that LRU (R1) and FIFO (R2) perform very similarly for these workloads, with LRU having slightly better performance. The difference between LRU and FIFO is minimal, suggesting that for these specific cache configurations and workloads, the replacement policy choice has less impact than write policies.  
Thus, to reach the best perfomance, we will choose the combination of write-back and write-allocate.

##Conslusion
Based on the analysis above, but optimizing every factor to its best AAT, we get the following combination as the best:

**Number of Sets**: 2048  
**Associativity**: 4-way
**Block Size**: 32 bytes
**Write Policy**: write-back
**Write Allocation**: write-allocate
**Replacement Policy**: lru
**Total Cache Size**: 256 KB

This configuration provides the optimal balance between performance and complexity, achieving high hit rates while maintaining reasonable hardware implementation costs.
