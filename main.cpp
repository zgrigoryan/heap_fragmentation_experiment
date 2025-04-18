#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#define MAX_ALLOC_SIZE 1024 * 1024  // Maximum allocation size (1MB)
#define MIN_ALLOC_SIZE 1024         // Minimum allocation size (1KB)
#define NUM_ALLOCATIONS 100         // Number of allocations in the loop

int main() {
    std::vector<void*> allocations;

    // Seed random number generator
    std::srand(std::time(0));

    for (int i = 0; i < NUM_ALLOCATIONS; ++i) {
        // Random allocation size between MIN_ALLOC_SIZE and MAX_ALLOC_SIZE
        size_t alloc_size = MIN_ALLOC_SIZE + std::rand() % (MAX_ALLOC_SIZE - MIN_ALLOC_SIZE);

        // Allocate memory and add it to the allocations vector
        void* ptr = malloc(alloc_size);
        if (ptr == nullptr) {
            std::cerr << "Memory allocation failed!" << std::endl;
            return 1;
        }
        allocations.push_back(ptr);

        // Deallocate every few iterations to simulate fragmentation
        if (i % 2 == 0 && !allocations.empty()) {
            free(allocations.back());
            allocations.pop_back();
        }

        // Simulate some computation or delay
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Clean up remaining allocations
    for (auto ptr : allocations) {
        free(ptr);
    }

    return 0;
}
