#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <vector>
#include <typeinfo>
#include <fstream>
#include <random>
#include <ctime>
using namespace std;

map<char, bool> letras_chutadas;
vector<char> chutes_errados;
string palavra_secreta = "easteregg";
bool nao_acertou = true;
bool nao_enforcou = true;

bool letra_existe(char chute) {
    for (char letra : palavra_secreta) {
        if (chute == letra) {
            return true;
        }
    }
    return false;
}

void imprime_letras_acertadas() {
    for (char letra : palavra_secreta) {
        if (letras_chutadas[letra]) {
            cout << letra << " ";
        }
        else {
            cout << "_ ";
        }
    }
    cout << endl;
}

void imprime_chutes_errados() {
    cout << "Chutes Errados: ";
    for (char letra : chutes_errados) {
        cout << letra << " ";
    }
    cout << endl;
}

bool verifica_se_nao_acertou() {
    for (char letra : palavra_secreta) {
        if (!letras_chutadas[letra]) {
            return true;
        }
    }

    return false;
}

bool verifica_se_nao_enforcou() {
    if (chutes_errados.size() < 5) {
        return true;
    }
    else {
        return false;
    }
}

void imprime_cabecalho() {
    cout << "******************************" << endl;
    cout << "**Bem Vindo ao Jogo Da Forca**" << endl;
    cout << "******************************" << endl;
    cout << endl;
    cout << "Voce tera 5 chances para acertar a palavra secreta" << endl;
}

void chuta() {
    char chute = '\0';
    cout << "Digite seu chute: " << endl;
    string input;
    cin >> input;
    if (input.size() != 1) {
        cout << "Digite exatamente um unico caracter!";
    }
    else {
        chute = tolower(input[0]);
        letras_chutadas[chute] = true;
        if (letra_existe(chute)) {
            cout << "Acertou.O chute esta na palavra" << endl;
            nao_acertou = verifica_se_nao_acertou();
        }
        else {
            cout << "Errou.O chute nao esta na palavra" << endl;
            chutes_errados.push_back(chute);
            nao_enforcou = verifica_se_nao_enforcou();
        }
    }
}
    
vector<string> le_arquivo() {
    vector<string> palavras_do_arquivo;
    ifstream arquivo;
    arquivo.open("palavras.txt");

    if (arquivo.is_open()) {
        int quantidade_de_palavras;
        arquivo >> quantidade_de_palavras;

        for (int i = 0; i < quantidade_de_palavras; i++) {
            string palavra;
            arquivo >> palavra;
            palavras_do_arquivo.push_back(palavra);
        }

        arquivo.close();
        return palavras_do_arquivo;
    }
    else {
        cout << "Nao foi possivel localizar o banco de palavras!";
        system("pause");
        exit(0);
    }
}

void sorteia_palavra() {
    vector<string> palavras_do_arquivo = le_arquivo();
    srand(time(NULL));
    int posicao_da_palavra = rand() % palavras_do_arquivo.size();

    palavra_secreta = palavras_do_arquivo[posicao_da_palavra];
}

string converte_minusculo(string palavra) {
    string palavra_minusculo;
    for (char letra : palavra) {
        palavra_minusculo.push_back(tolower(letra));
    }

    return palavra_minusculo;
}

void salva_arquivo(vector<string> nova_lista) {
    ofstream arquivo;
    arquivo.open("palavras.txt");
    if (arquivo.is_open()) {
        arquivo << nova_lista.size() << endl;
        for (string palavra : nova_lista) {
            arquivo << palavra << endl;
        }

        arquivo.close();
    }
    else {
        cout << "Nao foi possivel localizar o banco de palavras" << endl;
        system("pause");
        exit(0);
    }
}

void adiciona_palavra() {
    bool resposta_nao_valida = true;
    while (resposta_nao_valida) {
        cout << "Deseja adicionar uma nova palavra ao banco de palavras?(s/n)" << endl;
        string resposta;
        cin >> resposta;
        if (resposta.size() != 1) {
            cout << "Digite exclusivamente um unico caracter" << endl;
        }
        else {
            char valor_resposta = resposta[0];
            if (tolower(valor_resposta) == 's') {
                vector<string> lista_palavras = le_arquivo();
                string nova_palavra;
                cout << "Digite a nova palavra: " << endl;
                cin >> nova_palavra;
                nova_palavra = converte_minusculo(nova_palavra);
                lista_palavras.push_back(nova_palavra);
                salva_arquivo(lista_palavras);
                resposta_nao_valida = false;
            }
            else if (tolower(valor_resposta) == 'n') {
                resposta_nao_valida = false;
            }
            else {
                cout << "Digito Invalido!" << endl;
            }
        }
    }
}

int main()
{
    sorteia_palavra();
    imprime_cabecalho();

    while (nao_acertou && nao_enforcou) {
        imprime_chutes_errados();
        imprime_letras_acertadas();
        chuta();
        cout << endl;
    }

    cout << "Fim de Jogo" << endl;
    cout << "A palavra secreta era " << palavra_secreta << endl;
    if (nao_acertou) {
        cout << "Voce Perdeu!" << endl;
    }
    else {
        cout << "Voce Ganhou!" << endl;
        adiciona_palavra();
    }
    system("pause");
}

