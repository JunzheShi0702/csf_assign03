#include <iostream>

int main(int argc, char **argv)
{
  // TODO: implement

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

  // std out - hard-coded, will be replaced with variables in the future, currently as a start up.
  std::cout << "Total loads: 0" << std::endl;
  std::cout << "Total stores: 0" << std::endl;
  std::cout << "Load hits: 0" << std::endl;
  std::cout << "Load misses: 0" << std::endl;
  std::cout << "Store hits: 0" << std::endl;
  std::cout << "Store misses: 0" << std::endl;
  std::cout << "Total cycles: 0" << std::endl;
  return 0;
}
