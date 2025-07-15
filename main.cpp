#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <limits>
#include <omp.h>

using namespace std;

const int INFINITO = numeric_limits<int>::max();


// Esta funcion convierte un string con formato de matriz en una matriz de enteros
vector<vector<int>> leerMatriz(const string& entrada) {
    vector<vector<int>> matriz;
    stringstream flujo(entrada);
    char c;
    int numero;
    vector<int> fila;

    while (flujo >> c) {
        if (isdigit(c)) {
            flujo.putback(c);
            flujo >> numero;
            fila.push_back(numero);
        } else if (c == ']') {
            if (!fila.empty()) {
                matriz.push_back(fila);
                fila.clear();
            }
        }
    }

    return matriz;
}


// Esta funcion implementa el paralelismo usando OpenMP
// Calcula las distancias minimas desde el nodo 'inicio' a todos los demas nodos del grafo
void dijkstra(const vector<vector<int>>& grafo, int inicio, vector<int>& distancias) {
    int n = grafo.size();
    vector<bool> visitado(n, false);
    distancias.assign(n, INFINITO);
    distancias[inicio] = 0;

    for (int i = 0; i < n - 1; ++i) {
        int actual = -1;

        // Seleccion paralela del nodo no visitado con menor distancia
        #pragma omp parallel
        {
            int minimo_local = INFINITO;
            int nodo_local = -1;

            #pragma omp for nowait
            for (int j = 0; j < n; ++j) {
                if (!visitado[j] && distancias[j] < minimo_local) {
                    minimo_local = distancias[j];
                    nodo_local = j;
                }
            }

            #pragma omp critical
            {
                if (minimo_local < (actual == -1 ? INFINITO : distancias[actual])) {
                    actual = nodo_local;
                }
            }
        }

        if (actual == -1) break;
        visitado[actual] = true;

        // Relajacion paralela de los vecinos del nodo actual
        #pragma omp parallel for
        for (int vecino = 0; vecino < n; ++vecino) {
            if (grafo[actual][vecino] && !visitado[vecino] && distancias[actual] != INFINITO &&
                distancias[actual] + grafo[actual][vecino] < distancias[vecino]) {

                #pragma omp critical
                {
                    if (distancias[actual] + grafo[actual][vecino] < distancias[vecino]) {
                        distancias[vecino] = distancias[actual] + grafo[actual][vecino];
                    }
                }
            }
        }
    }
}


// Funcion principal: procesa los argumentos de entrada, ejecuta el algoritmo y escribe el resultado en un archivo
int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Uso: " << argv[0] << " \"matriz\" \"inicio\" \"archivo_salida\"\n";
        return 1;
    }

    string matriz_entrada = argv[1];
    int nodo_inicio = stoi(argv[2]);
    string ruta_salida = argv[3];

    vector<vector<int>> grafo = leerMatriz(matriz_entrada);
    vector<int> distancias;

    dijkstra(grafo, nodo_inicio, distancias);

    ofstream archivo(ruta_salida);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo de salida.\n";
        return 1;
    }

    archivo << "Vertice\t\tDistancia desde el origen\n";
    for (size_t i = 0; i < distancias.size(); ++i) {
        archivo << i << "\t\t" << (distancias[i] == INFINITO ? -1 : distancias[i]) << "\n";
    }

    archivo.close();
    return 0;
}
