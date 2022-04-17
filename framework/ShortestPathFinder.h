#pragma once
#include <memory>
#include <vector>

class ShortestPathFinder
{
public:
    ShortestPathFinder();
    ~ShortestPathFinder();
public:
    void clear();
    bool addEdge(uint64_t from, uint64_t to, uint32_t distance);
    uint32_t search(uint64_t from, uint64_t to, std::vector<uint32_t>& list);
private:
    std::unique_ptr<class ShortestPathFinderData> data;
};

using ShortestPathFinderPointer = std::shared_ptr<ShortestPathFinder>;
