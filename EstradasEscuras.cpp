#include <iostream>
#include <vector>
#include <algorithm> // para usar o método sort

using namespace std;

class ConjuntoDisjunto {
    public:
        ConjuntoDisjunto(int Tam); // construtor recebe o parâmetro referente ao seu tamanho
        virtual ~ConjuntoDisjunto();
        int Busca(int V); // busca um vértice no conjunto
        void UniaoVert(int U, int V); //método para unir dois conjuntos de vértices
    protected:
        int *Vert; // ponteiro para cada "subconjunto"
        int *Rank; // ponterio que indica a ordem de profundidade dos elementos no conjunto
};

ConjuntoDisjunto::ConjuntoDisjunto(int Tam) {
    Vert = new int[Tam]; // inicializa vert e rank com tam
    Rank = new int[Tam];
    for(int i = 0; i < Tam; i++) {
        Vert[i] = i; // cada vértice i é associado a um conjunto i
        Rank[i] = 0; // inicializa com 0 pois há apenas o próprio elemento no conjunto
    }
}

ConjuntoDisjunto::~ConjuntoDisjunto() { // devo destruir os ponteiros
    delete[] Vert;
    delete[] Rank;
    Vert = nullptr;
    Rank = nullptr;
}

int ConjuntoDisjunto::Busca(int V) {
    if (V == Vert[V]) { // se V for a raiz do conjunto já retorno o indice correspondente
        return Vert[V];
    } else { // caso contrário
        return Busca(Vert[V]); // realiza a recursão para atualizar o indice até encontrar a raiz
    }
};

void ConjuntoDisjunto::UniaoVert(int U, int V) {
    U = Busca(U);  // enconta a raiz de U e V
    V = Busca(V);
    // fazendo o link
    if(Rank[U] < Rank[V]) {  // se o rank de U for menor que o de V
        Vert[V] = U;
    } else {
        if (Rank[U] > Rank[V]) { // no caso de V ser menor que U
            Vert[U] = V;
        } else { // caso sejam iguais
            Vert[U] = V;
            Rank[V]--; // decrementa a profundidade de V
        }
    }
}

class Grafo {
    public:
        Grafo(int V, int A); // recebe o número de junções e estradas
        virtual ~Grafo();
        void setA(int a) {this->NumArestas = a;};
        int getA() const {return this->NumArestas;}
        void setV(int v) {this->NumVertices= v;};
        int getV() const {return this->NumVertices;}
        void Aresta(int P, int Vert1, int Vert2); // insere uma aresta no grafo, composto pelas duas junções e o peso
        int Kruskal();
    protected:
        int NumVertices; // junções
        int NumArestas; // estradas
        vector<pair<int, pair<int, int> >> Arestas; // crio um vetor de pair entre um int e outro pair de inteiros, as duas junções e o peso - arestas
};

Grafo::Grafo(int V, int A){
    setV(V); // indico o numero de arestas e de vertices, estradas e junções, respectivamente
    setA(A);
}

Grafo::~Grafo() {
    Arestas.clear(); // removendo os elementos das Arestas para liberar memória
}

void Grafo::Aresta(int P, int Vert1, int Vert2) {
    Arestas.push_back(make_pair(P, make_pair(Vert1, Vert2))); // cria um novo pair com o peso e as junções envolvidas e coloca ao fim do vetor de arestas
}

int Grafo::Kruskal() {
    ConjuntoDisjunto conj(NumVertices); // cria um conjunto disjunto indicando o número de junções
    sort(Arestas.begin(), Arestas.end()); // ordena as arestas em ordem crescente de acordo com o peso
    int ValorMin = 0;
    int ConjuntosVertU, ConjuntosVertV;
    for(int i = 0; i < getA(); i++) { // percorro as arestas do grafo
        ConjuntosVertU = conj.Busca(Arestas[i].second.first); // encontro em quais conjuntos estão os dois vértices da aresta
        ConjuntosVertV = conj.Busca(Arestas[i].second.second);
        if (ConjuntosVertU != ConjuntosVertV) { // se estiverem em conjuntos diferentes faço a união
            ValorMin += Arestas[i].first; // e incremento o peso
            conj.UniaoVert(ConjuntosVertU, ConjuntosVertV);
        }
    }
    return ValorMin; // retorna o valor otimizado
};

int main() {
    int NumVerticesAux, NumArestasAux, Custo, VertAux1, VertAux2, Peso;
    while((cin >> NumVerticesAux >> NumArestasAux) && (!(NumVerticesAux == 0 && NumArestasAux == 0))) { // leio as junções e as estradas
        Custo = 0;                                          // irei parar o while quando ler 0 e 0 para as duas entradas
        Grafo grafo(NumVerticesAux, NumArestasAux); // entrego número de junções e estradas para o meu grafo
        for(int i = 0; i < grafo.getA(); i++) { // leio as estradas e seu respectivo peso em metros
            cin >> VertAux1 >> VertAux2 >> Peso;
            grafo.Aresta(Peso, VertAux1, VertAux2); // crio a aresta correspondente
            Custo += Peso; // incrementando o custo total de iluminar todas as estradas
        }
        Custo -= grafo.Kruskal(); // Kruskal calcula o que pode ser otimizado, logo basta subtrairde custo este valor
        cout << Custo << endl; // mostro o custo final apos a otimização
    }
    return 0;
};
