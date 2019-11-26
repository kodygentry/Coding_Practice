#define DEBUG
#define MAXN 1e9
#include <bits/stdc++.h>
#define PRIM 1
#define KRUSAL 2
#define DIJKSTRA 3
#define BELLMAN 4
#define FLOYD 5


using namespace std;
class Node {
    public : 
        int index;
        int parent; /* -1 for root */
        int key;
        Node () {
            index = 0; 
            parent = -1;
            key = MAXN;
        }
        Node (int i , int p = -1, int k = MAXN) {
            index = i;
            parent = p;
            key = k;
        }
};
class disjoint_set {
    public :
        struct n {
            int parent;
            int rank;
        };
        int total;
        struct n *sset;

        disjoint_set (int a) {
            total = a;
            sset = new struct n [total];
            for (int i = 0; i < total; ++i) {
                sset[i].parent = i;
                sset[i].rank = 0;
            }
        }
        int find (int u) {
            if (u != sset[u].parent) 
                sset[u].parent = find(sset[u].parent);
            return sset[u].parent;
        }
        void _union (int u, int v) {
            int u_set = find(u);
            int v_set = find(v);
            if (sset[u_set].rank < sset[v_set].rank) {
                sset[u_set].parent = v_set;
            } else if (sset[u_set].rank > sset[v_set].rank ) {
                sset[v_set].parent = u_set;
            } else {
                sset[u_set].parent = v_set;
                sset[v_set].rank++;
            }
        }
};

class edge {
    public :
        int weight;
        int v1;
        int v2;

        edge (int w, int a1, int a2) {
            weight = w;
            v1 = a1;
            v2 = a2;
        }
        bool operator<(const edge& rhs) {
            return this->weight < rhs.weight;
        }
};

class Graph {
    private :
        int **graph;
        int vertex;
        vector<bool> visited;
        Node *node;
    public :
        Graph (int total) {
            vertex = total;
            graph = new int* [vertex];  
            for (int i = 0; i < vertex; ++i) {
                graph[i] = (int *)new int* [vertex];
            }
            // initial not connected
            for (int i = 0; i < vertex; ++i) {
                for (int j = 0; j < vertex; ++j) {
                    if (i == j) graph[i][j] = 0;
                    else graph[i][j] = MAXN;
                }
            }
            node = new Node [vertex];
            visited.resize(vertex, false);
        }
        void build () {
            int total_path;
            cin >> total_path;
            for (int i = 0; i < total_path; ++i) {
                int from, to;
                cin >> from >> to ;
                cin >> graph[from][to];
                graph[to][from] = graph[from][to];
            }
        }
        void dfs () {
            cout << "recursive : ";
            dfs_recursive(0);
            cout << endl;
            /* --------------------------*/
            fill(visited.begin(), visited.end(), false);
            cout << "nonrecursive : ";
            dfs_nonrecursive();
            cout << endl;
        }
        void dfs_recursive(int index) {
            visited[index] = true;
            cout << (char)(index+'A') << "-> ";
            for (int i = 0; i < vertex; ++i) {
                if (graph[index][i] != MAXN && visited[i] == false) {
                    dfs_recursive(i);
                }
            }
        }
        void dfs_nonrecursive() {
            // O(n^2)
            stack<int > buffer;
            buffer.push(0);
            while (!buffer.empty()) {
                int top = buffer.top();
                buffer.pop();
                if (!visited[top]) {
                    // visit top
                    cout << (char)(top+'A') << "-> ";
                    visited[top] = true;
                    // push the adjecent node into the stack
                    for (int i = vertex-1; i >= 0; --i) {
                        if (graph[top][i] != MAXN && visited[i] == false) {
                            buffer.push(i);
                        }
                    }
                }
            }
        }

        void bfs () {
            cout << "bfs " << endl;
            vector<bool> done; 
            done.resize(vertex, false);
            
            queue<int > q;
            q.push(0);
            while (!q.empty()) {
                int top = q.front();
                q.pop();
                if (!done[top]) { 
                    cout << (char)(top+'A') << "-> ";
                    done[top] = true;
                    for (int i = 0; i < vertex; ++i) {
                        if (graph[top][i] != MAXN && done[i] == false) {
                            q.push(i);
                        }
                    }
                }
            }
        }
        void spanning_tree (int method) {
            if (method == PRIM) prim();
            else if (method == KRUSAL) krusal();
        }
        void prim () {
            // initial
            vector<bool > gone(vertex);
            fill(gone.begin(), gone.end(), false);
            for (int i = 0; i < vertex; ++i) {
                node[i].index = i;
                node[i].key = MAXN;
                node[i].parent = -1;
            }
            node[vertex-1].key = 0;
            
            // prim
           for (int i = 0; i < vertex; ++i) {
                // find the min key node to deal 
                int min = MAXN;
                int min_index = -1;
                for (int j = 0; j < vertex; ++j) {
                    if (!gone[j] && min > node[j].key ) {
                        min = node[j].key;
                        min_index = j;
                    }
                }
                gone[min_index] = true;
                // update the near node's key
                for (int j = 0; j < vertex; ++j) {
                    if (graph[min_index][j] != MAXN &&
                        !gone[j] &&
                        node[j].key > graph[min_index][j]){
                        node[j].key = graph[min_index][j];
                        node[j].parent = min_index;
                    }
                }
           }

            // print result
            for (int i = 0; i < vertex; ++i) {
                cout << (char)(node[i].parent + 'A') << "--> " << (char)(node[i].index + 'A') << endl;
            }
            

        }
        void krusal() {
            vector<edge > sorted;
            for (int i = 0; i < vertex; ++i) {
                for (int j = i+1; j < vertex; ++j) {
                    if (graph[i][j] !=  MAXN) { 
                        sorted.push_back(edge(graph[i][j], i, j)); 
                    }
                }
            }
            sort(sorted.begin(), sorted.end());
            disjoint_set ans(vertex);
            vector<edge > ans_edge;

            // for each edge
            for (int i = 0; i < vertex; ++i) {
                // if the edge node not cause a cycle
                if (ans.find(sorted[i].v1) != ans.find(sorted[i].v2)) {
                    ans_edge.push_back(sorted[i]);
                    ans._union(sorted[i].v1, sorted[i].v2);
                }
            }
            for (int i = 0; i < ans_edge.size(); ++i) {
                cout << (char)(ans_edge[i].v1 + 'A') << "--> " << (char)(ans_edge[i].v2+'A') << endl;
            }
        }
        void single_source_shortest_path (int method, int start) {
            if (method == DIJKSTRA) dijkstra(start);
            else if (method == BELLMAN) bellman(start);
            else if (method == FLOYD) floyd(start);
        }
        void dijkstra (int start) {
            vector<int > cost(vertex);
            fill(cost.begin(), cost.end(), MAXN);
            vector<bool > gone(vertex);
            fill(gone.begin(), gone.end(), false);
            cost[start] = 0;

            for (int i = 0; i < vertex;++i) {
                // find the minmum cost of unjudge node
                int min = MAXN + 10;
                int min_index = -1;
                for (int j = 0; j < vertex; ++j) {
                    if (cost[j] < min && !gone[j]) {
                        min = cost[j];
                        min_index = j;
                    }
                }
                // mark visited
                gone[min_index] = true;
                // adjecent node of min
                for (int j = 0; j < vertex; ++j) {
                    if (graph[min_index][j] != MAXN &&
                        !gone[j] &&
                        min+graph[min_index][j] < cost[j]) {
                        cost[j] = min + graph[min_index][j];
                    }
                }
            }

            // print 
            cout << "from " << (char)(start+'A') << " to every node's cost is:\n";
            for (int i = 0; i < vertex; ++i) {
                if (i != start) {
                    cout << (char)(start+'A') << "--> " << (char)(i+'A') << " is " << cost[i] << endl;
                }
            }
        }
        void bellman (int start) {
            vector<int > pi(vertex);
            fill(pi.begin(), pi.end(), -1);
            vector<int > cost(vertex);
            fill(cost.begin(), cost.end(), MAXN);
            cost[start] = 0;

            for (int i = 0; i < vertex-1; ++i) {
                // for every node 
                for (int n = 0; n < vertex; ++n) {
                    // for its adjecent node
                    for (int adjecent = 0; adjecent < vertex; ++adjecent) {
                        if (graph[n][adjecent] != MAXN && 
                            cost[n] + graph[n][adjecent] < cost[adjecent] ) {
                            pi[adjecent] = n;
                            cost[adjecent] = cost[n] + graph[n][adjecent];
                        }
                    }
                }
            }

            // print
            cout << "from " << (char)(start+'A') << " to every node's cost is:\n";
            for (int i = 0; i < vertex; ++i) {
                if (i != start) {
                    cout << (char)(start+'A') << "--> " << (char)(i+'A') << " is " << cost[i] << endl;
                }
            }
        }
        void floyd (int source) {
            int cost[vertex][vertex];
            //copy map
            for (int i = 0; i < vertex;++i) {
                for(int j = 0; j < vertex; ++j) {
                    cost[i][j] = graph[i][j];
                }
            }
            for (int middle = 0; middle < vertex; ++middle) {
                for (int start = 0; start < vertex; ++start) {
                    for (int end = 0; end < vertex; ++end ) {
                        if (cost[start][middle] + cost[middle][end] < cost[start][end] ) {
                            cost[start][end] = cost[start][middle] + cost[middle][end] ;
                        }
                    }
                }
            }
            // print
            cout << "from " << (char)(source+'A') << " to every node's cost is:\n";
            for (int i = 0; i < vertex; ++i) {
                if (i != source) {
                    cout << (char)(source+'A') << "--> " << (char)(i+'A') << " is " << cost[source][i] << endl;
                }
            }
        }

};

int main () 
{
    #ifdef DEBUG
    freopen ("in.in", "r", stdin);
    #endif 
    int total;
    cin >> total;
    Graph t(total);

    t.build();
    //t.dfs();
    //t.bfs();
    //t.spanning_tree(PRIM); 
    //t.spanning_tree(KRUSAL); 
    
    cout << "dijkstra \n";
    t.single_source_shortest_path(DIJKSTRA, 0);
    cout << "bellman \n";
    t.single_source_shortest_path(BELLMAN, 0);
    cout << "floyd \n";
    t.single_source_shortest_path(FLOYD, 0);
    

    return 0;
}
