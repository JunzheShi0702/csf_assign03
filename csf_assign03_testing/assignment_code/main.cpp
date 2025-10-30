#include <iostream>
#include <vector>
#include <cmath>

// Single cache block
struct CacheBlock
{
  u_int32_t tag;
  bool valid;
  bool dirty; // for write-back policy specifically
  int order;
};

// Set of blocks
struct CacheSet
{
  std::vector<CacheBlock> cacheSet;
};

void handleSingleLoad(u_int32_t set_index,                                 // set index from address
                      u_int32_t tag,                                       // tag from address
                      std::vector<CacheSet> &cache,                        // cache passed by reference
                      int sets, int blocks, int block_size,                // cache parameters
                      int *load_hits, int *load_misses, int *total_cycles, // output stats passed by pointer
                      const std::string replacement_policy                 // replacement policy by reference
);

void handleSingleStore(u_int32_t set_index,                                   // set index from address
                       u_int32_t tag,                                         // tag from address
                       std::vector<CacheSet> &cache,                          // cache passed by reference
                       int sets, int blocks, int block_size,                  // cache parameters
                       int *store_hits, int *store_misses, int *total_cycles, // output stats passed by pointer
                       const std::string replacement_policy,                  // replacement policy
                       const std::string write_alloc,                         // write allocation policy
                       const std::string write_policy                         // write policy
);

u_int32_t getIndex(const u_int32_t address, const int index_bits, const int block_offset_bits);
u_int32_t getTag(const u_int32_t address, const int index_bits, const int block_offset_bits);

int main(int argc, char **argv)
{
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

  // Initialize cache sets and blocks
  for (int i = 0; i < sets; i++)
  {
    cache[i].cacheSet.resize(blocks); // Set size of each set to number of blocks
    for (int j = 0; j < blocks; j++)
    {
      cache[i].cacheSet[j].valid = false;
      cache[i].cacheSet[j].dirty = false; // only for write
      cache[i].cacheSet[j].order = 0;
      cache[i].cacheSet[j].tag = 0;
    }
  }

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

    const u_int32_t addr = std::stoul(address, nullptr, 16); // convert string address to u_int32_t

    // get number of bits for each part
    int index_bits = (int)log2(sets);
    int block_offset_bits = (int)log2(block_size);
    const u_int32_t tag = getTag(addr, index_bits, block_offset_bits);
    const u_int32_t set_index = getIndex(addr, index_bits, block_offset_bits);

    // operation string check, this determines the following logic
    if (operation == 's') // could only be s or l
    {
      total_stores++;
      handleSingleStore(set_index, tag, cache, sets, blocks, block_size,
                        &store_hits, &store_misses, &total_cycles,
                        replacement_policy, write_alloc, write_policy);
    }
    else // operation is l
    {
      // TODO by Junzhe Shi
      total_loads++;
      handleSingleLoad(set_index, tag, cache, sets, blocks, block_size,
                       &load_hits, &load_misses, &total_cycles,
                       replacement_policy);
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

// Helper functions to extract index and tag from address
u_int32_t getIndex(const u_int32_t address, const int index_bits, const int block_offset_bits)
{
  u_int32_t index_mask = (1 << index_bits) - 1;
  return (address >> block_offset_bits) & index_mask;
}

u_int32_t getTag(const u_int32_t address, const int index_bits, const int block_offset_bits)
{
  return address >> (index_bits + block_offset_bits);
}

// Actual implementation: only Hit and Miss two cases, for every hit no matter it is LRU, MRU or not, you can always do check things below and increment them, then set back to 0

// 0 - MRU, 3 - LRU

// Workflow notes - demo

// check command line valid or not (e.g. write-back + no-write-allocate combo is invalid)

// Load:
// 1. parse the input to get address
// 2. go to the set under that index
// 3. check whether tag exists by iterate through blocks in the set
//  3.1 exists then hit, update statistics (cycle + 1, hit + 1), update the order depends on fifo or LRU
//  3.2 does not exist, then miss (miss + 1, cycle + 100 * (block_size/4) + 1)
// 4. if does not exists, then check full or not （traverse to check number of valid blocks < total number of blocks）
// 5. if not full, set the empty block with current data, and valid token to true, update its order to 0 and also update other block's order(e.g. +1)
// 6. if full do lru replacement (FIFO for MS3) (replace the tag in the evicted CacheBlock) then update its order to 0 and also update other block's order
void handleSingleLoad(u_int32_t set_index, u_int32_t tag,
                      std::vector<CacheSet> &cache,
                      int sets, int blocks, int block_size,
                      int *load_hits, int *load_misses, int *total_cycles,
                      const std::string replacement_policy)
{
  bool exist = false;
  CacheSet &current_set = cache[set_index];
  for (int i = 0; i < blocks; i++)
  {
    if (current_set.cacheSet[i].tag == tag && current_set.cacheSet[i].valid)
    {
      exist = true;      // exist then hit, and set to true for skipping miss replacement
      (*load_hits)++;    // cache hit
      (*total_cycles)++; // 1 cycle for hit
      // update the order depends on fifo or LRU
      if (replacement_policy == "lru")
      {
        current_set.cacheSet[i].order = 0;
        for (int j = 0; j < blocks; j++)
        {
          if (j != i && current_set.cacheSet[j].valid)
          {
            current_set.cacheSet[j].order++;
          }
        }
      }
      break; // Exit loop after finding hit
    }
  }
  if (exist == false)
  {
    (*load_misses)++;                              // cache miss
    (*total_cycles) += 100 * (block_size / 4) + 1; // need memory access to load entire block + 1 cycle

    // check full or not
    bool is_full = true;
    for (int i = 0; i < blocks; i++)
    {
      if (current_set.cacheSet[i].valid == false)
      {
        // not full, use this block
        is_full = false;
        current_set.cacheSet[i].tag = tag;
        current_set.cacheSet[i].valid = true;
        current_set.cacheSet[i].order = 0; // set order to 0 as latest
        for (int j = 0; j < blocks; j++)
        {
          if (j != i && current_set.cacheSet[j].valid)
          {
            current_set.cacheSet[j].order++;
          }
        }
        break; // Exit loop after finding empty block
      }
    }

    if (is_full)
    {
      // full, need to evict one block based on replacement policy
      int evict_index = 0;
      if (replacement_policy == "lru")
      {
        int max_order = -1;
        for (int i = 0; i < blocks; i++)
        {
          if (current_set.cacheSet[i].order > max_order)
          {
            max_order = current_set.cacheSet[i].order;
            evict_index = i;
          }
        }
      }

      // if evicted block is dirty (write-back), add write-back penalty
      if (current_set.cacheSet[evict_index].dirty)
      {
        (*total_cycles) += 100 * (block_size / 4); // write entire dirty block back to memory
      }

      // replace the evicted block
      current_set.cacheSet[evict_index].tag = tag;
      current_set.cacheSet[evict_index].order = 0;
      current_set.cacheSet[evict_index].dirty = false; // load doesn't make it dirty

      // update other block's order
      for (int j = 0; j < blocks; j++)
      {
        if (j != evict_index && current_set.cacheSet[j].valid)
        {
          current_set.cacheSet[j].order++;
        }
      }
    }
  }
}
// Store:
// 1. parse the input to get address
// 2. go to the set under that index
// 3. check whether tag exists by iterate through blocks in the set
//  3.1 exists then hit, update statistics (cycle + 1, hit + 1), update the order
//    - Write-Through: Write to cache + write to memory (+ 100 * (block_size/4) cycles)
//    - Write-Back: Write to cache only, mark dirty (+0 extra cycles)
//    - update the order of every block depends on fifo or LRU
//  3.2 does not exist, then miss (miss + 1, cycle + 100 * (block_size/4) + 1)
//    - Write-Allocate: Bring block into cache
//        - if set is not full, update an empty block's tag and update its order to 0 and also update other block's order
//        - if set is full, decide which block to evict based on replacement policy, update the evicted block's tag and update its order to 0 and also update other block's order
//    - No-Write-Allocate: just add the cycled and skip the rest of the operations
void handleSingleStore(u_int32_t set_index, u_int32_t tag,
                       std::vector<CacheSet> &cache,
                       int sets, int blocks, int block_size,
                       int *store_hits, int *store_misses, int *total_cycles,
                       const std::string replacement_policy,
                       const std::string write_alloc,
                       const std::string write_policy)
{
  bool exist = false;
  CacheSet &current_set = cache[set_index];

  // check if tag exists in the set
  for (int i = 0; i < blocks; i++)
  {

    if (current_set.cacheSet[i].tag == tag && current_set.cacheSet[i].valid)
    {
      exist = true; // store hit
      (*store_hits)++;
      (*total_cycles)++; // 1 cycle for cache access

      // handle write policy
      if (write_policy == "write-through")
      {
        // write to cache + write to memory
        (*total_cycles) += 100; // 4 bytes max per access
      }
      else
      {
        // write to cache only, mark dirty
        current_set.cacheSet[i].dirty = true;
      }

      // update order for LRU (same as load)
      if (replacement_policy == "lru")
      {
        current_set.cacheSet[i].order = 0;
        for (int j = 0; j < blocks; j++)
        {
          if (j != i && current_set.cacheSet[j].valid)
          {
            current_set.cacheSet[j].order++;
          }
        }
      }
      break; // exit loop after finding hit
    }
  }

  // store miss
  if (exist == false)
  {
    (*store_misses)++;

    // for write-allocate: bring block into cache
    if (write_alloc == "write-allocate")
    {
      (*total_cycles) += 100 * (block_size / 4); // need memory access to bring entire block in

      // check if set is full
      bool is_full = true;
      for (int i = 0; i < blocks; i++)
      {
        if (current_set.cacheSet[i].valid == false)
        {
          // not full, use this block
          is_full = false;
          current_set.cacheSet[i].tag = tag;
          current_set.cacheSet[i].valid = true;
          current_set.cacheSet[i].order = 0;

          // set dirty if write-back and add cycle for write
          if (write_policy == "write-back")
          {
            current_set.cacheSet[i].dirty = true;
            (*total_cycles) += 1; // write to cache
          }
          else
          {
            (*total_cycles) += 1 + 100; // write-through: write to cache + memory (4 bytes max)
          }

          // Update other blocks order
          for (int j = 0; j < blocks; j++)
          {
            if (j != i && current_set.cacheSet[j].valid)
            {
              current_set.cacheSet[j].order++;
            }
          }
          break; // exit loop after finding empty block
        }
      }

      // set is full, need to evict
      if (is_full)
      {
        int evict_index = 0;
        if (replacement_policy == "lru")
        {
          int max_order = -1;
          for (int i = 0; i < blocks; i++)
          {
            if (current_set.cacheSet[i].order > max_order)
            {
              max_order = current_set.cacheSet[i].order;
              evict_index = i;
            }
          }
        }

        // if evicted block is dirty (write-back), add write-back penalty
        if (write_policy == "write-back" && current_set.cacheSet[evict_index].dirty)
        {
          (*total_cycles) += 100 * (block_size / 4); // write entire dirty block back to memory
        }

        // replace the evicted block
        current_set.cacheSet[evict_index].tag = tag;
        current_set.cacheSet[evict_index].order = 0;
        current_set.cacheSet[evict_index].dirty = (write_policy == "write-back");

        // add cycle for write after bringing block in
        if (write_policy == "write-back")
        {
          (*total_cycles) += 1; // write to cache
        }
        else
        {
          (*total_cycles) += 1 + 100; // write-through: write to cache + memory (4 bytes max)
        }

        // update other blocks' order
        for (int j = 0; j < blocks; j++)
        {
          if (j != evict_index && current_set.cacheSet[j].valid)
          {
            current_set.cacheSet[j].order++;
          }
        }
      }
    }
    else // no-write-allocate
    {
      // just write to memory, don't modify cache
      // Each store accesses at most 4 bytes, so 100 cycles per 4-byte quantity
      (*total_cycles) += 100;
    }
  }
}
