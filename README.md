# Recuperativo - Algoritmo de Dijkstra 

**Autor:** Ricardo Castillo Vega   
**Profesor:** Sebastián Salazar Molina  
**Universidad Tecnológica Metropolitana**  
**Fecha de entrega:** 15/07/2025

---

##Descripción

Este proyecto implementa el algoritmo de Dijkstra para encontrar la ruta más corta desde un vértice origen hacia todos los demás vértices de un grafo, 
utilizando paralelismo con OpenMP. La entrada se proporciona como matriz de adyacencia desde línea de comandos, y el resultado se guarda en un archivo de texto.

---

##Requisitos

- C++ (compilador `g++`)
- Soporte para OpenMP
- Sistema operativo: probado en **Ubuntu 24.04 LTS** y **Windows 10/11**

---

##Compilación 

```bash
g++ -fopenmp -o programa main.cpp

Ejecución
./programa "[[0,10,0,0,5],[0,0,1,0,2],[0,0,0,4,0],[7,0,6,0,0],[0,3,9,2,0]]" 0 "resultados.txt"
