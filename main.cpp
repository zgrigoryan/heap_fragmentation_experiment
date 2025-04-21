// heap_fragmentation_demo.cpp
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

constexpr std::size_t KB = 1024;
constexpr std::size_t MB = 1024 * KB;

constexpr std::size_t MIN_ALLOC = 1 * KB;   // 1 KB
constexpr std::size_t MAX_ALLOC = 1 * MB;   // 1 MB
constexpr int         N_ITER     = 5'000;   // more iterations → clearer graph
constexpr int         REPORT_EVERY = 250;   // print every N iterations

int main()
{
    std::cout << "Simulating heap fragmentation…\n"
              << "Will allocate " << N_ITER << " blocks, sizes "
              << MIN_ALLOC/KB << " KB–" << MAX_ALLOC/KB << " KB, "
              << "freeing randomly.\n\n";

    std::vector<void*> blocks;
    blocks.reserve(N_ITER);    

    std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<std::size_t> size_dist(MIN_ALLOC, MAX_ALLOC);
    std::uniform_int_distribution<int> free_dist(0, 1);  // 50 % chance to free a past block

    std::size_t total_allocated = 0, alive_blocks = 0;

    for (int i = 1; i <= N_ITER; ++i)
    {
        std::size_t bytes = size_dist(rng);
        void* ptr = std::malloc(bytes);
        if (!ptr) { std::perror("malloc"); return 1;}
        std::cout << "Allocated: " << bytes << " bytes\n";  
        blocks.push_back(ptr);

        blocks.push_back(ptr);
        total_allocated += bytes;
        ++alive_blocks;

        if (free_dist(rng) && !blocks.empty())
        {
            std::uniform_int_distribution<std::size_t>
                victim_dist(0, blocks.size() - 1);
            std::size_t idx = victim_dist(rng);
            std::free(blocks[idx]);
            blocks[idx] = blocks.back();
            blocks.pop_back();
            --alive_blocks;
        }

        if (i % REPORT_EVERY == 0)
        {
            std::cout << "Iter " << std::setw(5) << i << '/'
                      << N_ITER
                      << " | live blocks: " << std::setw(5) << alive_blocks
                      << " | cum. allocated ≈ "
                      << std::setw(6) << (total_allocated / MB) << " MB\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    for (void* p : blocks) std::free(p);

    std::cout << "\nDone. All remaining blocks freed. "
                 "Press q in top/htop to quit.\n";
}
