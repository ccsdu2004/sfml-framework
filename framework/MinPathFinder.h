#include <memory>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_tree/json_parser.hpp>

class MinPathFinder
{
public:
    MinPathFinder();
    ~MinPathFinder();
public:
    void clear();
    uint32_t addVertex();
    void addEdge(uint64_t from, uint64_t to, uint32_t distance);
    std::tuple<std::vector<uint32_t>, uint32_t> search(uint64_t from, uint64_t to);
private:
    std::unique_ptr<class MinPathFinderData> data;
};
