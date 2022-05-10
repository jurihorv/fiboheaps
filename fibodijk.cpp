#include <bits/stdc++.h>

using namespace std;

struct Node{
  int key, index, degree;
  Node *child, *parent; 
  Node *left, *right;
  bool mark;
  Node(){
    mark = false; degree = 0;
    left = right = this;
    child = parent = nullptr;
  }
};

class FibonacciHeap{
  private:
    Node *head; //pointer na minimalni   
    int N; //trenutni broj vrhova u cijeloj hrpi  
    unordered_map<int, Node*> mapa; //mapiranje (index, pointer)
    int getMaxDegree(){
      //ogradujemo D(n) sa log2(N)
      if(N == 0) return 1;
      assert(N > 0);
      int ret = (int)log2(N) + 1;
      return ret; 
    }

  public:
    FibonacciHeap(){
      N = 0; head = nullptr;
    }

    Node* Search(int index){
      if(mapa.find(index) == mapa.end()) 
        return nullptr;
      return mapa[index];
    }

    void HeapInsert(int index, int key){
      Node *novi = new Node;
      novi -> key = key;
      novi -> index = index;
      mapa[index] = novi;
      if(head == nullptr)
        head = novi;
      else{
        Node *tmp = head -> left;
        tmp -> right = novi;
        novi -> left = tmp;
        novi -> right = head;
        head -> left = novi; 
        if((head -> key) > (novi -> key))
          head = novi;
      }

      N++;
    }

    Node* ExtractMin(){
      Node *mini = head;
      if(mini != nullptr){
        mapa.erase(mini -> index);
        Node *x = mini -> child;
        Node *pamti = x; 
        do{
          if(x == nullptr) break;
          //stavim djete u root listu
          Node *next = x -> right;
          Node *tmp = head -> left;
          tmp -> right = x;
          x -> left = tmp; 
          x -> right = head;
          head -> left = x;
          x -> parent = nullptr;
          x = next;
        }while(x != pamti);

        //makni head iz strukture
        Node *prije = head -> left, *poslije = head -> right;
        prije -> right = poslije;
        poslije -> left = prije;
        if(mini == mini -> right){
          head = nullptr;
        }else{
          head = poslije;
          Consolidate();
        }

        N -= 1;
      }

      if(mini != nullptr){
        mini -> child = nullptr;
        mini -> left = mini -> right = mini;
      }

      return mini;
    }

    void HeapLink(Node *y, Node *x){
      Node *prije = y -> left, *poslije = y -> right;
      prije -> right = poslije;
      poslije -> left = prije;
      y -> parent = x;
      y -> mark = false;
      x -> degree++;

      assert(y -> key >= x -> key);

      if(x -> child == nullptr){
        x -> child = y;
        y -> left = y -> right = y;
      }else{
        Node *tmp = x -> child;
        prije = tmp -> left;
        y -> right = tmp;
        y -> left = prije;
        prije -> right = y;
        tmp -> left = y;
      }
    }
  
    //ovo treba samo za debugiranje
    vector<Node*> getRoots(){
      Node *tmp = head;
      Node *pamti = tmp;
      vector<Node*> ret;
      do{
        if(tmp == nullptr) break;
        Node *next = tmp -> right;
        ret.push_back(tmp);
        tmp = next;
      }while(tmp != pamti);

      return ret;
    }

    void Consolidate(){
      int maxi = getMaxDegree();
      vector<Node*> A(maxi + 1, nullptr);
      vector<Node*> rootovi = getRoots();

      int R = (int)rootovi.size();
      for(int i = 0; i < R; i++){
        if(rootovi[i] -> parent != nullptr)
          continue;
        Node *x = rootovi[i];
        int d = x -> degree;
        while(A[d] != x && A[d] != nullptr){
          Node *y = A[d];
          if(x -> key > y -> key)
            swap(x, y);
          HeapLink(y, x);
          A[d] = nullptr;
          d++;
        }

        A[d] = x;
      }

      head = nullptr;
      for(int i = 0; i <= maxi; i++){
        if(A[i] == nullptr)
          continue;
        if(head == nullptr){
          head = A[i]; 
          head -> left = head -> right = head;
          head -> parent = nullptr;
          continue;
        }

        Node *tmp = head -> left;
        tmp -> right = A[i];
        A[i] -> left = tmp;
        A[i] -> right = head;
        head -> left = A[i];
        A[i] -> parent = nullptr;
        if((head -> key) > (A[i] -> key))
          head = A[i];
      }
    }

    void DecreaseKey(int index, int val){
      Node *x = Search(index);
      if(x == nullptr){
        cout << "cvor ne postoji vise " << endl;
        return;
      }
      if(x -> key < val){
        cout << "ne mogu ga povecat " << endl;
        return;
      }

      x -> key = val;
      Node *y = x -> parent;
      if(y != nullptr && (x -> key) < (y -> key)){
        Cut(x, y);
        CascadingCut(y);
      }
      if(x -> key < head -> key){
        head = x;
      }
    }

    void Cut(Node *x, Node *y){
      Node *child = y -> child;
      //prvo izbacim x 
      if(child -> right == child && child -> left == child){
        assert(child == x);
        y -> child = nullptr;
      }else{
        Node *prije = x -> left, *poslije = x -> right;
        prije -> right = poslije;
        poslije -> left = prije;
        y -> child = poslije;
      }

      //onda x stavimo u root listu
      Node *tmp = head -> left;
      tmp -> right = x;
      x -> left = tmp;
      x -> right = head;
      head -> left = x;

      y -> degree -= 1;
      x -> mark = false;
      x -> parent = nullptr;
    }

    void CascadingCut(Node *y){
      Node *z = y -> parent;
      if(z != nullptr){
        if(y -> mark == false)
          y -> mark = true;
        else{
          Cut(y, z);
          CascadingCut(z);
        }
      }
    }
    
    Node* Minimum(){
      return head;
    }

    void Delete(int index){
      if(mapa.find(index) == mapa.end())
        return;
      DecreaseKey(index, INT_MIN);
      ExtractMin();
    }

    friend FibonacciHeap Union(FibonacciHeap H1, FibonacciHeap H2);
};

FibonacciHeap Union(FibonacciHeap H1, FibonacciHeap H2){
  FibonacciHeap ret;
  ret.head = H1.head;
  vector<Node*> root2 = H2.getRoots();
  for(auto r : root2){
    Node *tmp = (H1.head) -> left;
    tmp -> right = r;
    r -> left = tmp;
    r -> right = H1.head;
    (H1.head) -> left = r;
  }
  if(H1.head == nullptr || (H2.head != nullptr && (H2.head) -> key < (H1.head) -> key))
    ret.head = H2.head;
  ret.N = H1.N + H2.N;
  return ret;
}

void solve(){
 // freopen("in1", "r", stdin);
 // freopen("out1", "w", stdout);
  int V, E;
  cin >> V >> E; 

  vector<vector<pair<int, int>>> edge(V + 1); 
  for(int i = 0; i < E; i++){
    int u, v, c; cin >> u >> v >> c;
    edge[u].push_back(make_pair(v, c)); 
    edge[v].push_back(make_pair(u, c)); //ovu liniju treba komentirati ako radimo s usmjerenim grafovima
  }

  int source;
  cin >> source;

  FibonacciHeap heap;
  for(int i = 1; i <= V; i++){
    int c = i == source ? 0 : INT_MAX; 
    heap.HeapInsert(i, c);
  }
      
  vector<int> dist(V + 1, INT_MAX);
  dist[source] = 0;

  while(true){
    Node *najblizi = heap.ExtractMin();
    if(najblizi == nullptr) break;
    int cvor = najblizi -> index;
    int cost = najblizi -> key;
    //cout << "najblizi: " << cvor << " " << cost << endl;
    dist[cvor] = cost; 
    if(cost == INT_MAX) continue; //do ovog ne postoji put, nije od koristi 
    for(int j = 0; j < (int)edge[cvor].size(); j++){
      int c = edge[cvor][j].second;
      int nei = edge[cvor][j].first;
      Node *x = heap.Search(nei);
      if(x == nullptr) continue; //vec rijesen
      if((x -> key) > cost + c){
        heap.DecreaseKey(nei, cost + c);
      }
    }
  }

  for(int i = 1; i <= V; i++){
    if(dist[i] == INT_MAX)
      dist[i] = -1;
    if(i != source)
      cout << dist[i] << " ";
  }
  cout << endl;

}

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  //int test; cin >> test;
  int test = 1;
  while(test--)
    solve();

  return 0;
}
