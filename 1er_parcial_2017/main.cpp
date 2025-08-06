#include <cstring>
#include <iomanip>
#include <iostream>
using namespace std;

typedef char str20[21];
typedef unsigned short ushort;
const ushort MAX_TEMAS = 100;

struct sVotos {
    str20 nomTema;
    ushort cantVotos;
};

struct sEnc {
    char sexo;  // 'M' o 'V'
    ushort edad;
    str20 nomTema;
};

typedef sVotos tvVotos[MAX_TEMAS];

short busBinVec(tvVotos vVotos, str20 nomTema, ushort card) {
    int ult = card - 1;
    int prim = 0;
    int med;
    while (prim <= ult) {
        med = (prim + ult) / 2;
        if (strcmp(vVotos[med].nomTema, nomTema) == 0)
            return med;
        else if (strcmp(vVotos[med].nomTema, nomTema) < 0)
            prim = med + 1;
        else
            ult = med - 1;
    }
    return -1;
}

void IntCmb(sVotos &elem1, sVotos &elem2) {
    sVotos aux = elem1;
    elem1 = elem2;
    elem2 = aux;
}

void OrdxBur(tvVotos vVotos, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i <= card - k; i++)
            if (vVotos[i].cantVotos < vVotos[i + 1].cantVotos) {
                ordenado = false;
                IntCmb(vVotos[i], vVotos[i + 1]);
            }
    } while (!ordenado);
}

void inicvVotos(tvVotos vVotos, ushort &cantTemas) {
    cout << "Ingrese la cantidad de temas: ";
    cin >> cantTemas;
    cin.ignore();

    for (ushort i = 0; i < cantTemas; i++) {
        cout << "Tema " << i + 1 << ": ";
        cin.get(vVotos[i].nomTema, 21);
        cin.ignore();
        vVotos[i].cantVotos = 0;
    }
}

void obtDatos(sEnc &rEnc) {
    cout << "\nIngrese su sexo ('M', 'V', '*'): ";
    cin >> rEnc.sexo;

    if (rEnc.sexo != '*') {
        cout << "Ingrese su edad (>= 15): ";
        cin >> rEnc.edad;
        cin.ignore();
        cout << "Ingrese su tema: ";
        cin.getline(rEnc.nomTema, 21);
    } else {
        rEnc.edad = 0;
        strcpy(rEnc.nomTema, "*");
    }
}

void acumVotos(tvVotos vVotos, sEnc rEnc, ushort cantTemas) {
    short pos = busBinVec(vVotos, rEnc.nomTema, cantTemas);

    if (pos == -1)
        return;

    bool acumular = false;

    if (rEnc.sexo == 'M' && rEnc.edad >= 30 && rEnc.edad <= 50)
        acumular = true;

    if (rEnc.sexo == 'V' && (rEnc.edad < 30 || rEnc.edad > 50))
        acumular = true;

    if (acumular)
        vVotos[pos].cantVotos++;
};

void listadoTemasMusicales(tvVotos vVotos, ushort cantTemas) {
    OrdxBur(vVotos, cantTemas);

    cout << left << setw(7) << "Puesto" << " " << setw(21) << "Nombre de Tema"
         << " " << setw(6) << "Votos" << "\n";

    for (ushort i = 0; i < cantTemas; i++) {
        cout << left << setw(7) << i + 1 << " " << setw(21) << vVotos[i].nomTema
             << " " << setw(6) << vVotos[i].cantVotos << "\n";
    }
};

int main() {
    tvVotos vVotos;
    sEnc rEnc;
    ushort cantTemas;

    inicvVotos(vVotos, cantTemas);
    obtDatos(rEnc);

    while (rEnc.sexo != '*') {
        acumVotos(vVotos, rEnc, cantTemas);
        obtDatos(rEnc);
    }

    listadoTemasMusicales(vVotos, cantTemas);
    return 0;
}
