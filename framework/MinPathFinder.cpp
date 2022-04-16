#include "MinPathFinder.h"

typedef boost::property<boost::edge_weight_t, uint32_t> EdgeWeightProperty;
typedef boost::property<boost::vertex_index_t, uint32_t> VertexProperties;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
        VertexProperties, EdgeWeightProperty> Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

class MinPathFinderData
{
public:
    Graph graph;
    boost::property_map<Graph, boost::edge_weight_t>::type edge_distance;
};

MinPathFinder::MinPathFinder():
    data(new MinPathFinderData)
{
    data->edge_distance =
        boost::get(boost::edge_weight, data->graph);

    static_assert (sizeof(Vertex) == sizeof(uint64_t), "sizeof(Vertex) == sizeof(uint64_t)");
}

MinPathFinder::~MinPathFinder()
{
}

void MinPathFinder::clear()
{
    data->graph.clear();
}

uint32_t MinPathFinder::addVertex()
{
    return boost::add_vertex(data->graph);
}

void MinPathFinder::addEdge(uint64_t from, uint64_t to, uint32_t distance)
{
    Edge e = (boost::add_edge(from, to, data->graph)).first;
    data->edge_distance[e] = distance;
}

std::tuple<std::vector<uint32_t>, uint32_t> MinPathFinder::search(uint64_t from,
                                                                  uint64_t to)
{
    std::vector<Vertex> p(num_vertices(data->graph));
    std::vector<uint32_t> d(num_vertices(data->graph));
    Vertex s = vertex(from, data->graph);

    dijkstra_shortest_paths(data->graph, s, boost::predecessor_map(&p[0]).distance_map(&d[0]));

    std::vector<uint32_t> list;
    uint32_t distance;

    boost::graph_traits < Graph>::vertex_iterator vi, vend;
    for (std::tie(vi, vend) = vertices(data->graph); vi != vend; ++vi) {
        list.push_back(*vi);
        if (*vi == to) {
            distance = d[*vi];
            break;
        }
    }

    return std::make_tuple(list, distance);
}
