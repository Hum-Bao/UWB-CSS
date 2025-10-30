/**
 * A graph is made up of vertices and edges.
 * Vertex labels are unique.
 * A vertex can be connected to other vertices via weighted, directed edge.
 * A vertex cannot connect to itself or have multiple edges to the same vertex
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <array>
#include <map>
#include <string>
#include <vector>
#include "edge.h"
#include "vertex.h"

using namespace std;

static const int ADJ_MAX_SIZE = 100;

static const int STATE_NULL = -1;
static const int STATE_DEFAULT = 0;
static const int STATE_QUEUED = 1;
static const int STATE_VISITED = 2;
static const int STATE_MAX = 999;

class Graph {
 public:
    //default empty constructor
    Graph() = default;

    // constructor, empty graph
    explicit Graph(bool directionalEdges);

    // copy not allowed
    Graph(const Graph& other) = delete;

    // move not allowed
    Graph(Graph&& other) = delete;

    // assignment not allowed
    Graph& operator=(const Graph& other) = delete;

    // move assignment not allowed
    Graph& operator=(Graph&& other) = delete;

    /** destructor, delete all vertices and edges */
    ~Graph();

    void swap(const string& first_vertex, const string& second_vertex) noexcept;

    // @return true if vertex added, false if it already is in the graph
    bool add(const string& label);

    // @return true if vertex is in the graph
    [[nodiscard]] bool contains(const string& label) const;

    // @return total number of vertices
    [[nodiscard]] int verticesSize() const;

    // Get location of vertex in array
    // returns vertex location if it exists, or -1 if it does not
    [[nodiscard]] int getIndex(const string& name) const;

    // Checks if there is an edge between two vertices
    // returns true if there is, false if there is not
    [[nodiscard]] bool isAdjacent(int u_index, int v_index) const;

    // Add an edge between two vertices, create new vertices if necessary
    // A vertex cannot connect to itself, cannot have P->P
    // For digraphs (directed graphs), only one directed edge allowed, P->Q
    // Undirected graphs must have P->Q and Q->P with same weight
    // @return true if successfully connected
    bool connect(const string& from, const string& to, int weight = 0);

    // Remove edge from graph
    // @return true if edge successfully deleted
    bool disconnect(const string& from, const string& to);

    // @return total number of edges
    [[nodiscard]] int edgesSize() const;

    // @return number of edges from given vertex, -1 if vertex not found
    [[nodiscard]] int vertexDegree(const string& label) const;

    // @return string representing edges and weights, "" if vertex not found
    // A-3->B, A-5->C should return B(3),C(5)
    [[nodiscard]] string getEdgesAsString(const string& label) const;

    // Read edges from file
    // first line of file is an integer, indicating number of edges
    // each line represents an edge in the form of "string string int"
    // vertex labels cannot contain spaces
    // @return true if file successfully read
    bool readFile(const string& filename);

    // depth-first traversal starting from given startLabel
    void dfs(const string& startLabel, void visit(const string& label));

    // breadth-first traversal starting from startLabel
    // call the function visit on each vertex label */
    void bfs(const string& startLabel, void visit(const string& label));

    // Finds vertex with minimum path_
    // returns index of vertex with minimum path
    [[nodiscard]] int getMinVertex() const;

    //Dijktra Algorithm helper method
    void dijkstraAlgorithm(int start_index) const;

    //Dijktra Algorithm helper method
    void findPath(int start_index, int vertex_index, map<string, int>& weights,
                  map<string, string>& previous) const;

    // dijkstra's algorithm to find shortest distance to all other vertices
    // and the path to all other vertices
    // Path cost is recorded in the map passed in, e.g. weight["F"] = 10
    // How to get to the vertex is recorded previous["F"] = "C"
    // @return a pair made up of two map objects, Weights and Previous
    [[nodiscard]] pair<map<string, int>, map<string, string>> dijkstra(
        const string& startLabel) const;

    // Prim Algorithm helper method
    void primAlgorithm(int index);

    // minimum spanning tree using Prim's algorithm
    // ONLY works for NONDIRECTED graphs
    // ASSUMES the edge [P->Q] has the same weight as [Q->P]
    // @return length of the minimum spanning tree or -1 if start vertex not
    int mstPrim(const string& startLabel,
                void visit(const string& from, const string& to, int weight));

    // Kruskal Algorithm helper method
    void kruskalAlgorithm() const;

    // minimum spanning tree using Kruskal's algorithm
    // ONLY works for NONDIRECTED graphs
    // ASSUMES the edge [P->Q] has the same weight as [Q->P]
    // @return length of the minimum spanning tree or -1 if start vertex not
    int mstKruskal(const string& startLabel,
                   void visit(const string& from, const string& to,
                              int weight)) const;

 private:
    int num_vertex_ = 0;
    int num_edges_ = 0;
    std::array<std::array<int, ADJ_MAX_SIZE>, ADJ_MAX_SIZE> adj_matrix_ =
        std::array<std::array<int, ADJ_MAX_SIZE>, ADJ_MAX_SIZE>();
    std::array<Vertex*, ADJ_MAX_SIZE> vertex_list_ =
        std::array<Vertex*, ADJ_MAX_SIZE>();
    std::array<Edge, ADJ_MAX_SIZE> edge_list_ =
        std::array<Edge, ADJ_MAX_SIZE>();
    bool directed_ = true;
    //bfs helper method
    void bfsTraversal(int vertex_index, void visit(const string& label));
    //dfs helper method
    void dfsTraversal(int vertex_index, void visit(const string& label));
};

#endif  // GRAPH_H