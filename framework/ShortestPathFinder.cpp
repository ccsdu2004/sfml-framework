#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <ShortestPathFinder.h>
#include <iostream>

typedef boost::adjacency_list < boost::listS, boost::vecS, boost::directedS, boost::no_property, boost::property < boost::edge_weight_t, unsigned long > > graph_t;
typedef boost::graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
typedef boost::graph_traits < graph_t >::edge_descriptor edge_descriptor;

class ShortestPathFinderData
{
public:
    graph_t graph;
    boost::property_map<graph_t, boost::edge_weight_t>::type edgeWeightMap;

    void buildPath(uint32_t from, uint32_t to, std::vector<vertex_descriptor>& predecessor, std::vector<uint32_t>& output);
};

ShortestPathFinder::ShortestPathFinder():
    data(new ShortestPathFinderData)
{
    data->edgeWeightMap = boost::get(boost::edge_weight, data->graph);
}

ShortestPathFinder::~ShortestPathFinder()
{
}

void ShortestPathFinder::clear()
{
    data->graph.clear();
    data->edgeWeightMap = boost::get(boost::edge_weight, data->graph);
}

bool ShortestPathFinder::addEdge(uint64_t from, uint64_t to, uint32_t distance)
{
    edge_descriptor edge;
    bool inserted;
    boost::tie(edge, inserted) = boost::add_edge(from, to, data->graph);
    data->edgeWeightMap[edge] = distance;
    return inserted;
}

uint32_t ShortestPathFinder::search(uint64_t from, uint64_t to, std::vector<uint32_t> &list)
{
    std::vector<vertex_descriptor> predecessor(boost::num_vertices(data->graph));
    std::vector<uint32_t> distances(boost::num_vertices(data->graph), std::numeric_limits<uint32_t>::max());

    vertex_descriptor source = boost::vertex(from, data->graph);

    boost::property_map<graph_t, boost::vertex_index_t>::type pmpIndexmap = boost::get(boost::vertex_index, data->graph);
    boost::dijkstra_shortest_paths(data->graph, source, &predecessor[0], &distances[0], data->edgeWeightMap, pmpIndexmap,
                                   std::less<uint32_t>(), boost::closed_plus<uint32_t>(),
                                   std::numeric_limits<uint32_t>::max(), 0, boost::default_dijkstra_visitor());

    data->buildPath(from, to, predecessor, list);

    return list.empty() ? 0 : distances[to];
}

void ShortestPathFinderData::buildPath(uint32_t from, uint32_t to, std::vector<vertex_descriptor> &predecessor, std::vector<uint32_t> &output)
{
    output.clear();

    uint32_t oldTo = ~0;
    while(from != to) {
        output.push_back(to);
        to = predecessor[to];

        if(to == oldTo)
            return;

        oldTo = to;
    }

    output.push_back(to);
    std::reverse(output.begin(), output.end());
}
