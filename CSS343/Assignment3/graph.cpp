#include "graph.h"
#include <algorithm>
#include <array>
#include <fstream>
// #include <functional>
#include <iostream>
// #include <limits>
#include <map>
#include <queue>
// #include <set>
// #include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#include "vertex.h"

using namespace std;

// constructor, empty graph
// directionalEdges defaults to true
Graph::Graph(bool directionalEdges) : directed_(directionalEdges) {
    for (int outer = 0; outer < ADJ_MAX_SIZE; outer++) {
        for (int inner = 0; inner < ADJ_MAX_SIZE; inner++) {
            adj_matrix_[outer][inner] = 0;
        }
    }
}

// destructor
Graph::~Graph() {
    for (int count = 0; count < num_vertex_; count++) {
        delete vertex_list_[count];
    }
}

// @return total number of vertices
int Graph::verticesSize() const {
    return num_vertex_;
}

// @return total number of edges
int Graph::edgesSize() const {
    return num_edges_;
}

// @return number of edges from given vertex, -1 if vertex not found
int Graph::vertexDegree(const string& label) const {
    const int source_index = getIndex(label);
    if (source_index == -1) {
        return -1;
    }

    int degree = 0;
    for (int count = 0; count < num_vertex_; count++) {
        if (adj_matrix_[source_index][count] != 0) {
            degree++;
        }
    }
    return degree;
}

void Graph::swap(const string& first_vertex,
                 const string& second_vertex) noexcept {
    const int first_index = getIndex(first_vertex);
    const int second_index = getIndex(second_vertex);
    if (first_index == -1 || second_index == -1) {
        return;
    }

    std::swap(vertex_list_[first_index], vertex_list_[second_index]);

    for (int count = 0; count < num_vertex_; ++count) {
        std::swap(adj_matrix_[first_index][count],
                  adj_matrix_[second_index][count]);

        std::swap(adj_matrix_[count][first_index],
                  adj_matrix_[count][second_index]);
    }
}

// @return true if vertex added, false if it already is in the graph
bool Graph::add(const string& label) {
    if (num_vertex_ + 1 >= ADJ_MAX_SIZE) {
        return false;
    }
    if (getIndex(label) != -1) {
        return false;
    }
    vertex_list_[num_vertex_++] = new Vertex(label);
    return true;
}

/** return true if vertex already in graph */
bool Graph::contains(const string& label) const {
    return (getIndex(label) != -1);
    // return true;
}

// @return string representing edges and weights, "" if vertex not found
// A-3->B, A-5->C should return B(3),C(5)
string Graph::getEdgesAsString(const string& label) const {

    const int source_index = getIndex(label);
    if (source_index == -1) {
        return "";
    }

    string edges;

    for (int count = 0; count < num_vertex_; count++) {
        if (adj_matrix_[source_index][count] != 0) {
            edges += vertex_list_[count]->name_ + "(" +
                     std::to_string(adj_matrix_[source_index][count]) + "),";
        }
    }
    if (!edges.empty()) {
        edges.pop_back();
    }
    return edges;
}

// Get location of vertex in array
// returns vertex location if it exists, or -1 if it does not
int Graph::getIndex(const string& name) const {
    for (int count = 0; count < num_vertex_; count++) {
        if (name == vertex_list_[count]->name_) {
            return count;
        }
    }
    return -1;
}

// Add an edge between two vertices, create new vertices if necessary
// A vertex cannot connect to itself, cannot have P->P
// For digraphs (directed graphs), only one directed edge allowed, P->Q
// Undirected graphs must have P->Q and Q->P with same weight
// @return true if successfully connected
bool Graph::connect(const string& from, const string& to, int weight) {
    const int source_index = getIndex(from);
    const int destination_index = getIndex(to);
    if (source_index == destination_index ||
        adj_matrix_[source_index][destination_index] != 0) {
        return false;
    } else {
        adj_matrix_[source_index][destination_index] = weight;
        if (!directed_) {
            adj_matrix_[destination_index][source_index] = weight;
        }
        num_edges_++;
        return true;
    }
}

// Remove edge from graph
// @return true if edge successfully deleted
bool Graph::disconnect(const string& from, const string& to) {
    const int source_index = getIndex(from);
    const int destination_index = getIndex(to);

    if (adj_matrix_[source_index][destination_index] != 0) {
        adj_matrix_[source_index][destination_index] = 0;
        if (!directed_) {
            adj_matrix_[destination_index][source_index] = 0;
        }
        num_edges_--;
        return true;
    } else {
        return false;
    }
}

// Checks if there is an edge between two vertices
// returns true if there is, false if there is not
bool Graph::isAdjacent(int u_index, int v_index) const {
    return (adj_matrix_[u_index][v_index] != 0);
}

void Graph::dfsTraversal(int vertex_index, void visit(const string& label)) {
    stack<int> stack;

    stack.push(vertex_index);

    while (!stack.empty()) {
        vertex_index = stack.top();
        stack.pop();

        if (vertex_list_[vertex_index]->state_ == STATE_DEFAULT) {
            vertex_list_[vertex_index]->state_ = STATE_VISITED;
            visit(vertex_list_[vertex_index]->name_);
        }

        for (int count = num_vertex_ - 1; count >= 0; count--) {
            if (isAdjacent(vertex_index, count) &&
                vertex_list_[count]->state_ == STATE_DEFAULT) {
                stack.push(count);
            }
        }
    }
}

// depth-first traversal starting from given startLabel
void Graph::dfs(const string& startLabel, void visit(const string& label)) {
    if (getIndex(startLabel) == -1) {
        return;
    }
    for (int count = 0; count < num_vertex_; count++) {
        vertex_list_[count]->state_ = STATE_DEFAULT;
    }
    dfsTraversal(getIndex(startLabel), visit);
}

//bfs helper method
void Graph::bfsTraversal(int vertex_index, void visit(const string& label)) {
    queue<int> queue;

    queue.push(vertex_index);
    vertex_list_[vertex_index]->state_ = STATE_QUEUED;

    while (!queue.empty()) {
        vertex_index = queue.front();
        queue.pop();
        vertex_list_[vertex_index]->state_ = STATE_VISITED;

        visit(vertex_list_[vertex_index]->name_);

        for (int count = 0; count < num_vertex_; count++) {
            if (isAdjacent(vertex_index, count) &&
                vertex_list_[count]->state_ == STATE_DEFAULT) {
                queue.push(count);
                vertex_list_[count]->state_ = STATE_QUEUED;
            }
        }
    }
}

// breadth-first traversal starting from startLabel
void Graph::bfs(const string& startLabel, void visit(const string& label)) {
    if (getIndex(startLabel) == -1) {
        return;
    }

    for (int count = 0; count < num_vertex_; count++) {
        vertex_list_[count]->state_ = STATE_DEFAULT;
    }

    bfsTraversal(getIndex(startLabel), visit);
}

// Finds vertex with minimum path_
// returns index of vertex with minimum path
int Graph::getMinVertex() const {
    int min = STATE_MAX;
    int vertex_index = STATE_NULL;

    for (int count = 0; count < num_vertex_; count++) {
        if (vertex_list_[count]->state_ == STATE_DEFAULT &&
            vertex_list_[count]->path_ < min) {
            min = vertex_list_[count]->path_;
            vertex_index = count;
        }
    }

    return vertex_index;
}

//Dijktra Algorithm helper method
void Graph::dijkstraAlgorithm(int start_index) const {
    for (int count = 0; count < num_vertex_; count++) {
        vertex_list_[count]->state_ = STATE_DEFAULT;
        vertex_list_[count]->prev_ = STATE_NULL;
        vertex_list_[count]->path_ = STATE_MAX;
    }

    vertex_list_[start_index]->path_ = 0;

    while (true) {
        const int current = getMinVertex();

        if (current == STATE_NULL) {
            break;
        }

        vertex_list_[current]->state_ = STATE_QUEUED;

        for (int vertex_index = 0; vertex_index < num_vertex_; vertex_index++) {
            if (isAdjacent(current, vertex_index) &&
                vertex_list_[vertex_index]->state_ == STATE_DEFAULT) {
                if ((vertex_list_[current]->path_ +
                     adj_matrix_[current][vertex_index]) <
                    vertex_list_[vertex_index]->path_) {
                    vertex_list_[vertex_index]->prev_ = current;
                    vertex_list_[vertex_index]->path_ =
                        vertex_list_[current]->path_ +
                        adj_matrix_[current][vertex_index];
                }
            }
        }
    }
}

//Dijktra Algorithm helper method
void Graph::findPath(int start_index, int vertex_index,
                     map<string, int>& weights,
                     map<string, string>& previous) const {

    const int index = vertex_index;
    array<int, ADJ_MAX_SIZE> path = array<int, ADJ_MAX_SIZE>();
    int shortest_distance = 0;
    int count = 0;
    int u_index = 0;

    while (vertex_index != start_index) {
        count++;
        path[count] = vertex_index;
        u_index = vertex_list_[vertex_index]->prev_;
        shortest_distance += adj_matrix_[u_index][vertex_index];
        vertex_index = u_index;
    }

    count++;
    path[count] = start_index;
    weights.insert({vertex_list_[index]->name_, shortest_distance});

    if (index > 0) {
        previous.insert(
            {vertex_list_[index]->name_, vertex_list_[index - 1]->name_});
    } else {
        previous.insert(
            {vertex_list_[index]->name_, vertex_list_[start_index]->name_});
    }
}

// store the weights in a map
// store the previous label in a map
pair<map<string, int>, map<string, string>> Graph::dijkstra(
    const string& startLabel) const {
    map<string, int> weights;
    map<string, string> previous;

    const int start_index = getIndex(startLabel);

    if (start_index == -1) {
        return make_pair(weights, previous);
    }

    dijkstraAlgorithm(start_index);
    for (int vertex_index = 0; vertex_index < num_vertex_; vertex_index++) {
        if (vertex_list_[vertex_index]->path_ != STATE_MAX) {
            findPath(start_index, vertex_index, weights, previous);
        }
    }
    weights.erase(startLabel);
    previous.erase(startLabel);
    return make_pair(weights, previous);
}

// Prim Algorithm helper method
void Graph::primAlgorithm(int index) {
    int count = 0;

    for (int counter = 0; counter < num_vertex_; counter++) {
        vertex_list_[counter]->state_ = STATE_DEFAULT;
        vertex_list_[counter]->prev_ = STATE_NULL;
        vertex_list_[counter]->path_ = STATE_MAX;
    }

    vertex_list_[index]->path_ = 0;

    while (true) {
        const int current = getMinVertex();

        if (current == STATE_NULL) {
            if (count == num_vertex_ - 1) {
                break;
            } else {
                return;
            }
        }
        vertex_list_[current]->state_ = STATE_QUEUED;

        if (current != index) {
            count++;
            edge_list_[count].u_ = vertex_list_[current]->prev_;
            edge_list_[count].v_ = current;
        }

        for (int vertex_index = 0; vertex_index < num_vertex_; vertex_index++) {
            if (isAdjacent(current, vertex_index) &&
                vertex_list_[vertex_index]->state_ == STATE_DEFAULT) {
                if ((adj_matrix_[current][vertex_index]) <
                    vertex_list_[vertex_index]->path_) {
                    vertex_list_[vertex_index]->prev_ = current;
                    vertex_list_[vertex_index]->path_ =
                        adj_matrix_[current][vertex_index];
                }
            }
        }
    }
}

// minimum spanning tree using Prim's algorithm
// ONLY works for NONDIRECTED graphs
// ASSUMES the edge [P->Q] has the same weight as [Q->P]
// @return length of the minimum spanning tree or -1 if start vertex not
int Graph::mstPrim(const string& startLabel,
                   void visit(const string& from, const string& to,
                              int weight)) {
    const int start_index = getIndex(startLabel);
    if (start_index == -1) {
        return -1;
    }
    int weight = 0;

    primAlgorithm(start_index);
    for (int count = 1; count <= num_vertex_ - 1; count++) {
        visit(vertex_list_[edge_list_[count].u_]->name_,
              vertex_list_[edge_list_[count].v_]->name_,
              adj_matrix_[edge_list_[count].u_][edge_list_[count].v_]);
        weight += adj_matrix_[edge_list_[count].u_][edge_list_[count].v_];
    }
    return weight;
}

// Kruskal Algorithm helper method
void Graph::kruskalAlgorithm() const {
    // TODO
}

// minimum spanning tree using Kruskal's algorithm
// ONLY works for NONDIRECTED graphs
// ASSUMES the edge [P->Q] has the same weight as [Q->P]
// @return length of the minimum spanning tree or -1 if start vertex not
int Graph::mstKruskal(const string& startLabel,
                      void visit(const string& from, const string& to,
                                 int weight)) const {
    int weight = 0;

    kruskalAlgorithm();

    for (int count = 1; count <= num_vertex_ - 1; count++) {
        weight += adj_matrix_[edge_list_[count].u_][edge_list_[count].v_];
    }

    return weight;
}

// read a text file and create the graph
bool Graph::readFile(const string& filename) {
    ifstream myfile(filename);
    if (!myfile.is_open()) {
        cerr << "Failed to open " << filename << "\n";
        return false;
    }
    int edges = 0;
    int weight = 0;
    string from_vertex;
    string to_vertex;
    myfile >> edges;
    vector<string> history;
    map<pair<string, string>, int> edge_list;
    for (int i = 0; i < edges; ++i) {
        myfile >> from_vertex >> to_vertex >> weight;
        history.push_back(from_vertex);
        history.push_back(to_vertex);
        edge_list.insert({{from_vertex, to_vertex}, weight});
    }
    myfile.close();
    sort(history.begin(), history.end());
    for (const string temp : history) {
        add(temp);
    }

    for (pair<pair<string, string>, int> temp : edge_list) {
        connect(get<0>(temp.first), get<1>(temp.first), temp.second);
    }
    return true;
}
