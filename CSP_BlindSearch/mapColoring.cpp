/**
Matheus Gomes da Silva Horta - 8532321
Lais Helena Chiachio -  8517032
**/
#include <iostream>
#include <string>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <vector>
#include <utility>
#include <ctime>
#define COLOR_NUM 4
using namespace std;

/**
Estrutura representando as informacoes de um vertice
    values : true ou false para cada valor do dominio
    remainingVal : quantidade de valroes remanescentes
    degree : grau do vertice
**/
typedef struct {

    bool *values;
    int remainingVal;
    int degree;

} Domain;

/**
Funcao booleana que checa se o char e um espaco ou ponto.
Usada para remover esses caracteres de um nome lido
**/
bool isCommaOrSpaceOrDot(char c) {

    switch(c) {
    //case ',':
    case '.':
    case ' ':
        return true;
    default:
        return false;
    }
}
/**
Funcao utilizada para checar no vector de estados, se o nome de um determinado estado
ja existe naquele vetor. Se existir, retorna o par com aquele estado e o indice dele. Se não,
cria um novo par, adiciona-o ao vetor e retorna esse par
@Args :
    pair<string,int> state : par contendo o estado e seu indice.
    vector<pair<string,int> > &states : referencia para o vetor de pares estados com os indices.
**/
pair<string,int> searchInVector(pair<string,int> state, vector<pair<string,int> > &states) {

    for( unsigned int j = 0; j < states.size(); j++) {

        if(get<0>(states[j]) == get<0>(state)) {
            return states[j];
        }
    }
    get<1>(state) = states.size();
    states.push_back(state);
    return state;
}
/**
Funcao que cria e retorna um grafo alocado dinamicamente **/
bool** allocateGraph(int vertexNum) {
    bool **Graph = new bool*[vertexNum];
    for (int i = 0; i < vertexNum ; ++i) {
        Graph[i] = new bool[vertexNum];
    }
    for( int i = 0; i < vertexNum; i ++) {
        for( int j = 0; j < vertexNum; j ++) {
            Graph[i][j] = false;
        }
    }
    return Graph;
}
/**
Funcao que deleta um grafo representado por matrix **/
void deleteMatrix (bool **Graph, int vertexNum) {
    for( int i = 0; i < vertexNum; i++ ){
        delete [] Graph[i];
    }
    delete [] Graph;
}
/**
Funcao que adiciona uma aresta entre dois vertices do grafo **/
void addEdge(bool **Graph, int source, int destiny) {

    Graph[source][destiny] = true;
    Graph[destiny][source] = true;
}
/**
Funcao auxiliar que imprime o grafo **/
void printfGraph(bool **Graph, int vertexNum) {
    cout << "\n";
    for (int i = 0; i < vertexNum; ++i) {
        for (int j = 0; j < vertexNum; ++j) {

           cout << Graph[i][j] << " ";
        }
        cout << "\n";
    }
}
/**
Funcao que imprime a solucao de acordo com a especificacao do trabalho
@Args:
    int *solution : Vetor com as solucoes.
    vector <  pair< string,int> > &colors : Referencia para um vetor de pares de cores com seus indices.
    vector < pair < string, int > > &States : Referencia para um vetor de pares estados com seus indices.
    int countriesNumber : Quantidade de paises ou estados.
    int *attribution : Quantidade de atribuicoes na execucao
    **/
void printSolution(int *solution, vector <  pair< string,int> > &colors, vector < pair < string, int > > &States,
                    int countriesNumber, int *attribution) {
    for( int i =0; i < countriesNumber; i ++) {
        cout << get<0>(States[i]) << ": " << get<0>(colors[solution[i]]) << ".\n";
    }
    cout << "\nAtribuicoes : " << *attribution;
}
/**
Funcao que retorna se determinado valor a ser atribuido a uma variavel é valido ou não para forwardChecking
@Args:
    bool **Country : Ponteiro para o grafo.
    int *subject : Ponteiro para o estado sendo avaliado.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
    int *color : Cor sendo avaliada.
**/
bool isValidFC(bool **Country, int *subject, Domain *domain, int *countrieNumber, int *solution, int *color) {
    if(domain[*subject].values[*color] == false) {
        return false;
    }
    for(int i=0; i < *countrieNumber; i ++) {

        if(Country[*subject][i] && solution[i] == *color) {
            return false;
        }
    }
    return true;
}
/**
Função que retorna o próximo valor a ser atribuido com o uso de Forward Checking
@Args:
    bool **Country : Ponteiro para o grafo.
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
**/
int getNextFC(bool **Country, int *solution, int *countriesNumber) {
    for(int i = 0; i < *countriesNumber; i++) {
        if(solution[i] == -1) {
            return i;
        }
    }
    return -1;
}
/**
Funcao utilitária usada no Forward Checking, que retira o valor atribuido a variável, de todas as variáveis de acordo com
as restrições. Em seguida, checa se algum domínio foi esgotado e retorna false se for o caso.
@Args:
    bool **Country : Ponteiro para o grafo.
    int *subject : Ponteiro para o estado sendo avaliado.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
    int *color : Cor sendo avaliada.
**/
bool FC_util(bool **Country, Domain *domain, int *solution, int *subject,
             int *countriesNumber, int *color) {

    //percorre os valores
    for(int i = 0; i < *countriesNumber; i++) {
            // se satisfaz condições
        if(Country[*subject][i] && solution[i] == -1) {
            // diminui o valor do dominio se ele for mudado
            if(domain[i].values[*color] == true) {
                domain[i].values[*color] = false;
                domain[i].remainingVal--;
            }
        }
    }

    for(int i =0; i < *countriesNumber; i ++) {
        if(Country[*subject][i] && solution[i] == -1 && domain[i].remainingVal == 0) {
            return false;
        }
    }
    return true;
}
/**
Funcao utilitária que realiza o oposto de FC_util, no caso de ter sido realizado algum backtracking
@Args:
    bool **Country : Ponteiro para o grafo.
    int *subject : Ponteiro para o estado sendo avaliado.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
    int *color : Cor sendo avaliada.
**/
void FC_util_reverse(bool **Country, Domain *domain, int *solution,
                     int *subject,int *countriesNumber, int *color) {
    for(int i = 0; i < *countriesNumber; i++) {
        if(Country[*subject][i] && solution[i] == -1) {
            if(domain[i].values[*color] == false) {
                domain[i].values[*color] = true;
                domain[i].remainingVal++;
            }
        }
    }
}
/**
Funcao que retorna o próximo valor a ser atribuido de acordo com FC+MRV
@Args:
    bool **Country : Ponteiro para o grafo.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
**/
int getNextFC_MRV(bool **Country, Domain *domain, int *solution, int *countriesNumber) {
    int subject = 99;
    for(int i = 0; i < *countriesNumber; i++) {
            // pega a variavel com menor valor remanescente
        if(domain[i].remainingVal < subject && solution[i] == -1) {
            subject = i;
        }
    }
    // se todas as variaveis foram atribuidas, return false
    if( subject == 99 ) {
        return -1;
    }
    return subject;
}
/**
Funcao que retorna o próximo valor a ser atribuido de acordo com FC+MRV+grau
@Args:
    bool **Country : Ponteiro para o grafo.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
**/
int getNextFC_MRV_grau(bool **Country, Domain *domain, int *solution, int *countriesNumber) {

    Domain subject;
    subject.remainingVal = 99;
    subject.degree = -99;
    int aux;

    for(int i = 0; i < *countriesNumber; i++) {
        // pega a variavel com menor valor remanescete e maior grau
        if(domain[i].remainingVal <= subject.remainingVal && solution[i] == -1) {
            if(domain[i].degree > subject.degree) {
                subject.remainingVal = domain[i].remainingVal;
                subject.degree = domain[i].degree;
                aux = i;
            }
        }
    }
    if( subject.remainingVal == 99 ) {
        return -1;
    }
    return aux;
}
/**
Funcao que representa a execução do backtracking com FC+MRV+grau
@Args:
    bool **Country : Ponteiro para o grafo.
    int *subject : Ponteiro para o estado sendo avaliado.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int qtdColors : quantidade de cores
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
    int *color : Cor sendo avaliada.
**/
bool backTracking_forwardCheck_MRV_grau(bool **Country, int *solution,Domain *domain, int qtdColors,
                               int countriesNumber, int *attribution) {

    // Se não existe variáveis sem atribuição, fim
    int subject = getNextFC_MRV_grau(Country,domain,solution,&countriesNumber);
    if(subject == -1) return true;

    for( int i =0; i < qtdColors; i ++) {
        //checa se é valido a atribução
        if(isValidFC(Country,&subject,domain,&countriesNumber,solution,&i) ) {

            (*attribution)++;
            solution[subject] = i;
            // Tira os valores dos domínios
            if(FC_util(Country,domain,solution,&subject,&countriesNumber,&i) == false) {
                // Se algum dominio zerou, tente o proximo valor
                solution[subject] = -1;
                FC_util_reverse(Country,domain,solution,&subject,&countriesNumber,&i);
                continue;
            }
            if( backTracking_forwardCheck_MRV_grau(Country,solution,domain,qtdColors,countriesNumber,attribution)) {
                return true;
            }
            solution[subject] = -1;
            FC_util_reverse(Country,domain,solution,&subject,&countriesNumber,&i);
        }
    }

    return false; // Backtracking
}

/**
Funcao que representa a execução do backtracking com FC+MRV
@Args:
    bool **Country : Ponteiro para o grafo.
    int *subject : Ponteiro para o estado sendo avaliado.
    Domain *domain : Ponteiro para a estrutura com as informacoes sobre o dominio da variavel sendo avaliada.
    int qtdColors : quantidade de cores
    int *countrieNumber : Quantidade de paises ou estados.
    int *solution : Vetor que armazena a solucao.
    int *color : Cor sendo avaliada.
**/
bool backTracking_forwardCheck_MRV(bool **Country, int *solution,Domain *domain, int qtdColors,
                               int countriesNumber, int *attribution) {

    int subject = getNextFC_MRV(Country,domain,solution,&countriesNumber);
    if(subject == -1) return true;

    for( int i =0; i < qtdColors; i ++) {
        //checa se é valido a atribução
        if(isValidFC(Country,&subject,domain,&countriesNumber,solution,&i) ) {

            (*attribution)++;
            solution[subject] = i;
            // Tira os valores dos domínios
            if(FC_util(Country,domain,solution,&subject,&countriesNumber,&i) == false) {
                // Se algum dominio zerou, tente o proximo valor
                solution[subject] = -1;
                FC_util_reverse(Country,domain,solution,&subject,&countriesNumber,&i);
                continue;
            }
            if( backTracking_forwardCheck_MRV(Country,solution,domain,qtdColors,countriesNumber,attribution)) {
                return true;
            }
            solution[subject] = -1;
            FC_util_reverse(Country,domain,solution,&subject,&countriesNumber,&i);
        }
    }

    return false;
}

bool backTracking_forwardCheck(bool **Country, int *solution,Domain *domain, int qtdColors,
                               int countriesNumber, int *attribution) {

    int subject = getNextFC(Country,solution,&countriesNumber);
    if(subject == -1) return true;

    for( int i =0; i < qtdColors; i ++) {
        // Se o valor é valido, atribua
        if(isValidFC(Country,&subject,domain,&countriesNumber,solution,&i) ) {

            (*attribution)++;
            solution[subject] = i;
            // Retira o valor do domínio das outras variaveis
            if(FC_util(Country,domain,solution,&subject,&countriesNumber,&i) == false) {
                // Se algum dominio zerou, retire o valor e tente o proximo
                solution[subject] = -1;
                FC_util_reverse(Country,domain,solution,&subject,&countriesNumber,&i);
                continue;
            }
            // va para a proxima variavel
            if( backTracking_forwardCheck(Country,solution,domain,qtdColors,countriesNumber,attribution)) {
                return true;
            }
            // retire o valor da variavel e volte os dominios e tente o proximo valor
            solution[subject] = -1;
            FC_util_reverse(Country,domain,solution,&subject,&countriesNumber,&i);
        }
    }
    return false; // backtracking
}

bool isValid(bool **Country, int *subject, int *countrieNumber, int *solution, int *color) {

    for(int i=0; i < *countrieNumber; i ++) {

        if(Country[*subject][i] && solution[i] == *color) {
            return false;
        }
    }
    return true;
}
bool backTracking(bool **Country, int *solution, int qtdColors,int countriesNumber, int *attribution) {

    // se não existem mais valores, retorne
    int subject = getNextFC(Country,solution,&countriesNumber);
    if(subject == -1) return true;

    for( int i =0; i < qtdColors; i ++) {
        // se é valido, atribua
        if(isValid(Country,&subject,&countriesNumber,solution,&i) ) {
            solution[subject] = i;
            (*attribution)++;
            // vai para o proximo
            if( backTracking(Country,solution,qtdColors,countriesNumber,attribution)) {
                return true;
            }
            // retire o valor e continue tentando
            solution[subject] = -1;
        }
    }
    return false; // backtracking
}
int main() {

    /// variaveis de uso geral
    int countriesNumber;                    // numero de estados
    char heuristic;                         // heuristica a ser usada
    int numOfColors = 4;                    // numero de cores
    int attribution = 0;                    // quatidade de atribuicoes a variaveis

    cin >> countriesNumber >> heuristic;    // le a primeira linha de entrada

    /// variaveis usadas na leitura de entradas
    // armazena uma linha da entrada
    string line;
    // armazena o estado principal ( ate os dois pontos ) e o inteiro para o seu indice
    pair<string,int> mainState;
    // armazena os estados vizinhos ( separados por virgula ) e o inteiro para seus indices
    pair<string,int> neighbour;
    // armazena todos os estados e seus indices
    vector < pair < string, int > > States;
    // stream a ser usada para se ler o estado principal
    stringstream ss1;
    // stream a ser usada para se ler os estados auxiliares
    stringstream ss2;
    // string para amazenar tudo o que vem depois dos dois pontos na entrada por linha
    string neighbourLine;

    /// variaveis usadas na execucao do backtracking
    // Grafo representado por matriz
    bool **Country = allocateGraph(countriesNumber);
    // vetor com as solucoes
    int *solution = new int[countriesNumber];
    // cada cor possivel e seus indices
    vector < pair < string, int > > colors;
    colors.push_back(make_pair("Azul",0));
    colors.push_back(make_pair("Amarelo",1));
    colors.push_back(make_pair("Verde",2));
    colors.push_back(make_pair("Vermelho",3));

    /// variaveis usadas na verificacao adiante
    Domain *domain = new Domain[countriesNumber];
    // ignora uma linha para evitar erros
    cin.ignore();

    /// leitura de entradas
    //enquanto existir algo para ler
    while( getline(cin,line) ) {
        // armazena a linha lida na stream
        ss1 << line;
        // le o estado principal e armazena em mainState
        getline(ss1, get<0>(mainState), ':');
        // apaga caracteres espaço e ponto do nome lido
        get<0>(mainState).erase(
            remove_if(get<0>(mainState).begin(),get<0>(mainState).end(), &isCommaOrSpaceOrDot),get<0>(mainState).end()
        );
        // checa se esse par<nome,indice> ja esta no vetor de estados
        mainState = searchInVector(mainState,States);

        // le o restando da stream e armazena na linha de estados vizinhos
        while (getline(ss1, neighbourLine, '.')) {
            // transfere essa linha para a stream
            ss2 << neighbourLine;
            // le cada estado vizinho separado por virgula e armazena em neighbour
            while(getline(ss2, get<0>(neighbour), ',')) {

                // apaga caracteres espaço e ponto do nome lido
                get<0>(neighbour).erase(
                    remove_if(get<0>(neighbour).begin(),get<0>(neighbour).end(), &isCommaOrSpaceOrDot),get<0>(neighbour).end()
                );
                // checa se esse par<nome,indice> esta no vetor de estados
                neighbour = searchInVector(neighbour,States);
                // adiciona uma aresta entre o estado principal e seu vizinho
                addEdge(Country,get<1>(mainState),get<1>(neighbour));
            }
            // limpa a strea
            ss2.clear();
        }
        ss1.clear();
    }

    // Inicializa os vetores e estrutura com valores iniciais
    for( int i = 0; i < countriesNumber; i++) {
        solution[i] = -1;
    }
    for (int i = 0; i < countriesNumber ; ++i) {
        domain[i].values = new bool[numOfColors];
        domain[i].remainingVal = COLOR_NUM;
    }
    for( int i = 0; i < countriesNumber; i ++) {
        for( int j = 0; j < numOfColors; j ++) {
            domain[i].values[j] = true;
        }
    }
    for( int i = 0; i < countriesNumber; i ++) {
        for( int j = i+1; j < countriesNumber; j ++) {
            if(Country[i][j]) {
                domain[i].degree++;
            }
        }
    }

    clock_t begin = clock(); // inica a contagem
    switch(heuristic) {

    case 'a':
        // backtracking comum
        if(backTracking(Country,solution,numOfColors,countriesNumber,&attribution) ) {
            printSolution(solution,colors,States,countriesNumber,&attribution);
        }
        else {
            cout << "\nNão existe solucao\n ";
        }
        break;
    case 'b':
        // backtracking + forward checking
        if(backTracking_forwardCheck(Country,solution,domain,numOfColors,countriesNumber,&attribution) ) {
            printSolution(solution,colors,States,countriesNumber,&attribution);
        }
        else {
            cout << "\nNão existe solucao\n ";
        }
        break;
    case 'c':

        // backtracking + forward checking + mrv

        if(backTracking_forwardCheck_MRV(Country,solution,domain,numOfColors,countriesNumber,&attribution) ) {
            printSolution(solution,colors,States,countriesNumber,&attribution);
        }
        else {
            cout << "\nNão existe solucao\n ";
        }

        break;
    case 'd':
        // bt + fc + mrv + grau
        if(backTracking_forwardCheck_MRV_grau(Country,solution,domain,numOfColors,countriesNumber,&attribution) ) {
            printSolution(solution,colors,States,countriesNumber,&attribution);
        }
        else {
            cout << "\nNão existe solucao\n ";
        }

        break;

    }
    clock_t end = clock(); // encerra a contagem
    deleteMatrix(Country,countriesNumber);
    delete [] solution;
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\nTempo : %.4f",elapsed_secs);

    return 0;
}
