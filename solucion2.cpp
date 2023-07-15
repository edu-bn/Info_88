#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <cmath>

using namespace std;

#define SIZE  69903
#define SIZED2 10000
#define inser 5000
#define FILE "D1.txt"
#define FILE2 "D2.txt"
#define k  8.0  //con .0 para cuando cre las grillas nItems/k aproxime bien el valor
#define TEST 1

struct ListaDoblementeEnlazada
{
    ListaDoblementeEnlazada *referencia;
    unsigned char *palabra;
    ListaDoblementeEnlazada *siguiente;
};
typedef struct ListaDoblementeEnlazada nodoDL;

bool insercionG(nodoDL **L1, nodoDL *Ll, unsigned char *palabra, int &nItems);
bool busqueda(nodoDL *L1, nodoDL *Ll, unsigned char *palabra);

bool mayorDeAmbas(unsigned char *palabra0, unsigned char *palabra1);
bool esIgualQue(unsigned char *palabra0, unsigned char *palabra1);

void appendToListR(nodoDL **L1, unsigned char *palabra, int &nItems);
void printList(nodoDL *L1);
int creaGrilla(nodoDL **Ll, int largoLAnterior);
unsigned char **arregloD2();
double calculaMilisegundos(int inicio, int fin);

int main()
{
    // Declaracion de variables.
    cout << "Creando grilla...." << endl;
    unsigned char *palabra;
    string wordArray;
    int contador = 0, nItems = 0;

    clock_t inicio, fin;
    double segundos;
    // Cabecera de nuestra grilla
    nodoDL *L1 = NULL;

    // SE INICIA EL RELOJ!
    inicio = clock();

    ifstream archivo(FILE);
    if (!archivo)
    {
        cout << "El archivo no esta presente en el directorio!" << endl;
        return EXIT_FAILURE;
    }
    else
    {
        contador = 0;
        while (archivo >> wordArray && contador < SIZE)
        {
            palabra = new unsigned char[wordArray.length()];

            for (long unsigned int i = 0; i < wordArray.length(); i++)
                palabra[i] = wordArray[i];

            // Funcion para crear la fila L1
            appendToListR(&L1, palabra, nItems);
            contador++;
        }

        archivo.close();
    }
    /*  Creamos el puntero Ll el cual estara ubicado en la ultima fila de abajo
        La variable largo se utiliza para ir creando las listas hacia habajo largo/k
         
    */
    nodoDL *Ll = L1;
    int largo;
    largo = creaGrilla(&Ll, nItems);
    cout << "L1: " << nItems << " items" << endl;

    while (largo > k)
    {
        cout << "    " << largo << " items" << endl;
        largo = creaGrilla(&Ll, largo);
    }
    cout << "Ll: " << largo << " items" << endl;


    fin = clock();
    segundos = double(fin - inicio) / CLOCKS_PER_SEC;
    cout << "La creacion de la grilla llevo: " << segundos << "segundos. " << endl;


    // Creamos el diccionario con 5000 palabras de D2 desordenado
    unsigned char **dic2Desordenado = arregloD2();


    // Testeo de funciones
    if (TEST)
    {
        bool seguir = true, encontrado, insertado;
        int palabras_encontradas, palabras_insertadas;
        char accion;

        while (seguir)
        {
            cout << "-- ACCIONES -->" << endl;
            cout << "Ingresa 'B' para buscar las 10000 palabras de D2 " << endl;
            cout << "Ingresa 'I' para insertar 5000 palabras de D2.txt" << endl;
            cout << "Ingresa 'S' para salir" << endl;
            cout << "<----" << endl;
            cout << "Accion: ";
            cin >> accion;
            cin.ignore();
            switch ((accion))
            {
            case 'B':
                palabras_encontradas = 0;
                inicio = clock();
                cout << "Buscando las 10000 palabras de D2: \n.   .   .   .   .   .\n";
                for (int i = 0; i < SIZED2; i++)
                {
                    encontrado = busqueda(L1, Ll, dic2Desordenado[i]);
                    if (encontrado)
                        palabras_encontradas++;
                }
                fin = clock();
                segundos = double(fin - inicio) / CLOCKS_PER_SEC;
                cout << "Se encontraron un total de " << palabras_encontradas << " palabras en un tiempo de: " 
                << calculaMilisegundos(inicio, fin) << " milisegundos." << endl << ".   .   .   .   .   .\n" << endl;
                break;

            case 'I':
                palabras_insertadas = 0;
                cout << "Insertando 5000 palabras de D2: \n.   .   .   .   .   .\n";
                inicio = clock();
                for (int i = 0; i < inser; i++)
                {
                    insertado = insercionG(&L1, Ll, dic2Desordenado[i], nItems);
                    if (insertado)
                        palabras_insertadas++;
                }
                fin = clock();
                cout << "Se insertaron un total de " << palabras_insertadas << " palabras en un tiempo de: " 
                 << calculaMilisegundos(inicio, fin) << " milisegundos." << endl << ".   .   .   .   .   .\n" << endl;
                break;

            case 'S':
                seguir = false;
                break;

            default:
                cout << "(!) Opcion no valida" << endl;
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}

void appendToListR(nodoDL **L1, unsigned char *palabra, int &nItems)
{
    nodoDL *nuevo = new nodoDL;
    nuevo->palabra = palabra;
    nuevo->siguiente = NULL;
    nuevo->referencia = NULL;
    if (*L1 == nullptr)
    {
        *L1 = nuevo;
        nItems++;
    }
    else
    {
        nodoDL *p = *L1;
        while (p->siguiente != nullptr)
            p = p->siguiente;
        p->siguiente = nuevo;
        nItems++;
    }
}

void printList(nodoDL *L1)
{
    nodoDL *p = L1;
    cout << " L-> ";
    while (p != nullptr)
    {
        cout << p->palabra << " -> ";
        p = p->siguiente;
    }
    cout << "NULL" << endl;
}

int creaGrilla(nodoDL **Ll, int largoLAnterior)
{
    nodoDL *p = *Ll;
    nodoDL *nuevo = new nodoDL;
    nuevo->palabra = p->palabra;
    nuevo->referencia = p;
    nuevo->siguiente = NULL;

    *Ll = nuevo;     // Acomodo el puntero Ll en la nueva lista abajo
    nodoDL *q = *Ll; // Dejo un puntero q en la cabecera de Ll para poder conectar los nodos que se crearan

    int largoLi = 1;
    int pivots = round(largoLAnterior / k);
    if (pivots == 1)
        pivots++;
    for (int j = 1; j < pivots; j++)
    {
        for (int i = 0; i < k; i++)
            p = p->siguiente;

        nodoDL *nuevo = new nodoDL;
        nuevo->palabra = p->palabra;
        nuevo->referencia = p;
        q->siguiente = nuevo;
        nuevo->siguiente = NULL;
        q = nuevo;
        largoLi++;
    }

    return largoLi;
}

bool mayorDeAmbas(unsigned char *palabra0, unsigned char *palabra1)
{
    int i;
    i = 0;
    while (palabra0[i] != '\0' && palabra1[i] != '\0')
    {
        if (palabra0[i] > palabra1[i])
            return true;
        else if (palabra0[i] < palabra1[i])
            return false;
        i++;
    }
    if (palabra0[i] == '\0')
    {
        return false;
    }
    return true;
}

bool esIgualQue(unsigned char *palabra0, unsigned char *palabra1)
{
    int i;
    i = 0;
    while ((palabra0[i] != '\0' && palabra1[i] != '\0') && (palabra0[i] == palabra1[i]))
        i++;
    if (palabra0[i] != palabra1[i])
        return false;
    return true;
}

bool insercionG(nodoDL **L1, nodoDL *Ll, unsigned char *palabraI, int &nItems)
{
    nodoDL *p = Ll;
    nodoDL *q = p->siguiente;
   

    while (p->referencia != nullptr || (mayorDeAmbas(palabraI, q->palabra) && q->siguiente != nullptr)  )
    {
        if (mayorDeAmbas(p->palabra, palabraI))
            p = *L1;
        else if (mayorDeAmbas(q->palabra, palabraI))
            p = p->referencia;
        else
        {
            if (q->siguiente == nullptr && p->referencia != nullptr)  
            {
                p = q;
                while (p->referencia != nullptr)
                    p = p->referencia;
            }
            else
                p = q;
        }
        q = p->siguiente;
    }
    if (esIgualQue(q->palabra, palabraI) || esIgualQue(p->palabra, palabraI))
        return false;

    //creacion del nodo a insertar despues de verificar que la palabra no esté
    nodoDL *nuevo = new nodoDL;
    nuevo->palabra = new unsigned char[strlen((const char*)palabraI) + 1];
    strcpy((char*)nuevo->palabra, (const char*)palabraI);
    nuevo->referencia = nuevo->siguiente = NULL;

    if (mayorDeAmbas(p->palabra, palabraI))
    {
        *L1 = nuevo;
        nuevo->siguiente = p;
        nItems++;
        return true;
    }
    else if (mayorDeAmbas(palabraI, q->palabra))
    {
        q->siguiente = nuevo;
        nItems++;
        return true;
    }
    else
    {
        p->siguiente = nuevo;
        nuevo->siguiente = q;
        nItems++;
        return true;
    }
    return false;
}

bool busqueda(nodoDL *L1, nodoDL *Ll, unsigned char *palabraB)
{
    nodoDL *p = Ll;
    nodoDL *q = p->siguiente;
    while (p != nullptr && q != nullptr)
    {
        if (esIgualQue(q->palabra, palabraB) || esIgualQue(p->palabra, palabraB))
            return true;

        else if (mayorDeAmbas(p->palabra, palabraB)) // palabra antes del primer pivote
        {
            
            p = L1;
            q = p->siguiente;
            if(mayorDeAmbas(p->palabra, palabraB)) // palabra antes de L1
                return false;
        }
        else if (mayorDeAmbas(q->palabra, palabraB))
        {
            p = p->referencia;
            if (p != nullptr)
                q = p->siguiente;
        }
        else if (mayorDeAmbas(palabraB, q->palabra) && q->siguiente == nullptr) // palabra despues del ultimo pivote
        {
            p = q;
            while (p->referencia != nullptr)
                p = p->referencia;
            q = p->siguiente;
        }
        else
        {
            p = q;
            q = q->siguiente; 
        }
    }
    return false;
}

unsigned char **arregloD2()
{
    unsigned char *palabra2;
    unsigned char **palabrasDes = new unsigned char *[SIZED2];
    string wordArray2;
    int contador2, posAl1, posAl2;
    long unsigned int i;



    ifstream archivo2(FILE2);
    if (!archivo2)
    {
        cout << "El archivo no esta presente en el directorio!" << endl;
        return NULL;
    }
    else
    {
        contador2 = 0;
        while (archivo2 >> wordArray2 && contador2 < SIZED2)
        {
            palabra2 = new unsigned char[wordArray2.length()];
            for (i = 0; i < wordArray2.length(); i++)
                    palabra2[i] = wordArray2[i];
                   
            palabra2[i] = '\0';

            
            palabrasDes[contador2] = palabra2;
         
            contador2++;
        }
        for(int j = 0; j < SIZED2; j++)
        {
            posAl1 = rand() % SIZED2;
            posAl2 = rand() % SIZED2;
            swap(palabrasDes[posAl1], palabrasDes[posAl2]);
        }
        archivo2.close();
        return palabrasDes;
    }
}

double calculaMilisegundos(int inicio, int fin)
{
    return double(fin - inicio) / CLOCKS_PER_SEC * 1000.0;
}
