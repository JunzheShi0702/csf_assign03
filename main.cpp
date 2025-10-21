#include <iostream>
#include <vector>

// Single cache block
struct CacheBlock
{
  u_int32_t tag;
  bool valid;
  bool dirty;      // for write-back policy specifically
  u_int32_t order; // for LRU/FIFO, since no bit shift is allowed we need to use actual time stamp
};

// Set of blocks
struct CacheSet
{
  std::vector<CacheBlock> cacheSet;
};

int main(int argc, char **argv)
{
  // TODO: implement
  // ################ BASIC SETUP AND INPUT VALIDATION ####################

  // parameter format:
  // ./csim 256 4 16 write-allocate write-back lru < sometracefile
  if (argc != 7)
  {
    std::cerr << "Usage: ./csim <sets> <assoc> <blocksize> <write-allocate|no-write-allocate> <write-through|write-back> <lru|fifo> < trace_file" << std::endl;
    return 1;
  }

  // parse parameters
  int sets = std::stoi(argv[1]);
  int blocks = std::stoi(argv[2]);
  int block_size = std::stoi(argv[3]);
  std::string write_alloc = argv[4];
  std::string write_policy = argv[5];       // through or back
  std::string replacement_policy = argv[6]; // lru or fifo

  // Input validation
  // TODO: more validation here
  // sets blocks block_size < 0, block_size >= 4
  if (sets <= 0 || blocks <= 0)
  {
    std::cerr << "Error: set number and block number must be positive";
    return 1;
  }

  if (block_size < 4)
  {
    std::cerr << "Error: Block size must be at least 4 bytes" << std::endl;
    return 1;
  }
  // sets blocks block_size power of 2
  if ((sets & (sets - 1)) != 0)
  {
    std::cerr << "Error: Number of sets must be a power of 2" << std::endl;
    return 1;
  }

  if ((blocks & (blocks - 1)) != 0)
  {
    std::cerr << "Error: Number of blocks per set must be a power of 2" << std::endl;
    return 1;
  }

  if ((block_size & (block_size - 1)) != 0)
  {
    std::cerr << "Error: Block size must be a power of 2" << std::endl;
    return 1;
  }

  // write alloc can only be write-allocate or no-write-allocate
  if (write_alloc != "write-allocate" && write_alloc != "no-write-allocate")
  {
    std::cerr << "Error: Write allocation policy must be 'write-allocate' or 'no-write-allocate'" << std::endl;
    return 1;
  }

  // write policy can only be write-through or write-back
  if (write_policy != "write-through" && write_policy != "write-back")
  {
    std::cerr << "Error: Write policy must be 'write-through' or 'write-back'" << std::endl;
    return 1;
  }

  // replacement policy can only be lru or fifo
  if (replacement_policy != "lru" && replacement_policy != "fifo")
  {
    std::cerr << "Error: Replacement policy must be 'lru' or 'fifo'" << std::endl;
    return 1;
  }

  // Invalid combination: write-back and no-write-allocate
  if (write_policy == "write-back" && write_alloc == "no-write-allocate")
  {
    std::cerr << "Error: Cannot combine 'write-back' with 'no-write-allocate'" << std::endl;
    return 1;
  }

  // ######################## OUTPUT VARIABLES #############################

  int total_loads = 0;
  int total_stores = 0;
  int load_hits = 0;
  int load_misses = 0;
  int store_hits = 0;
  int store_misses = 0;
  int total_cycles = 0;

  // ######################## SIMULATION LOGIC #############################

  /*
    Basic parameters information:
    argv[0] = set number
    argv[1] = block number in each set
    argv[2] = size of each block
    argv[3] = write-allocate/non-write-allocate
    argv[4] = write-through/write-back
    argv[5] = evict policy
  */

  // currently most understandable way is nested vector
  std::vector<CacheSet> cache(sets);

  std::string op_string;
  char operation;
  std::string address; // int/string?

  // read each line for operation information <operation> <address> <third-arg>
  while (std::getline(std::cin, op_string))
  {
    // split the string based on space
    size_t first_space = op_string.find(' '); // find first space, lies between operation char and address

    size_t second_space = op_string.find(' ', first_space + 1); // find second space, lies after the address, used for substring extraction

    operation = op_string[0];
    address = op_string.substr(first_space + 1, second_space - first_space - 1);

    // operation string check, this determines the following logic
    if (operation == 's') // could only be s or l
    {
    }
    else // operation is l
    {
    }
  }

  // NOTICE: the offset is not actually needed for cache simulation

  // ######################### OUTPUT SECTION ############################
  std::cout << "Total loads: " << total_loads << std::endl;
  std::cout << "Total stores: " << total_stores << std::endl;
  std::cout << "Load hits: " << load_hits << std::endl;
  std::cout << "Load misses: " << load_misses << std::endl;
  std::cout << "Store hits: " << store_hits << std::endl;
  std::cout << "Store misses: " << store_misses << std::endl;
  std::cout << "Total cycles: " << total_cycles << std::endl;
  return 0;
}

// Detailed Explanation for HIT OTHERS for LRU in Lecture 16: Increment counter below the original slot order value, then set the hit slot to 00.

// Actual implementation: only Hit and Miss two cases, for every hit no matter it is LRU, MRU or not, you can always do check things below and increment them, then set back to 0

// 0 - MRU, 3 - LRU

// Workflow notes - demo
// Load:
// 1. parse the input to get address
// 2. go to the set under that index
// 3. check whether tag exists by iterate through blocks in the set
//  3.1 exists then hit, update statistics (cycle + 1, hit + 1), update the order
//  3.2 does not exist, then miss (miss + 1, cycle + 100 * (block_size/4) + 1)
// 4. if does not exists, then check full or not （traverse to check length < blocks）
// 5. if not full, set the empty block with current data, and valid token to true, update order (current length + 1)
// 6. if full do lru replacement (FIFO for MS3) (new CacheBlock declared and replaced) then update order

// Store:
// 1. parse the input to get address
// 2. go to the set under that index
// 3. check whether tag exists by iterate through blocks in the set
//  3.1 exists then hit, update statistics (cycle + 1, hit + 1), update the order
//    - Write-Through: Write to cache + write to memory (+ 100 * (block_size/4) cycles)
//    - Write-Back: Write to cache only, mark dirty (+0 extra cycles)
//  3.2 does not exist, then miss (miss + 1, cycle + 100 * (block_size/4) + 1)
//    - Write-Allocate: Bring block into cache, then proceed to step 3.1
//    - No-Write-Allocate: Write directly to memory, STOP (skip step 3.1)
