/*
MUESTRA DE DATOS

Sexo     Edad     Tema
M        25       a       // NO suma
V        18       b       // suma
M        50       c       // suma
V        98       c       // suma
V        46       a       // NO suma

RESULTADOS ESPERADOS

Puesto   Tema     Votos
1        c        2
2        b        1
3        a        0
*/

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
    char sexo;
    ushort edad;
    str20 nomTema;
};

typedef sVotos tvVotos[MAX_TEMAS];

int busBinVec(tvVotos vVotos, str20 clave, int card) {
    int ult = card - 1;
    int prim = 0;
    int med;
    while (prim <= ult) {
        med = (prim + ult) / 2;
        if (strcmp(vVotos[med].nomTema, clave) == 0)
            return med;
        else if (strcmp(vVotos[med].nomTema, clave) < 0)
            prim = med + 1;
        else
            ult = med - 1;
    }
    return -1;
}

void intCmb(sVotos &elem1, sVotos &elem2) {
    sVotos aux = elem1;
    elem1 = elem2;
    elem2 = aux;
}

void ordxBur(tvVotos vVotos, ushort card) {
    ushort k = 0;
    bool ordenado;
    do {
        ordenado = true;
        k++;
        for (ushort i = 0; i < card - k; i++)
            if (vVotos[i].cantVotos < vVotos[i + 1].cantVotos) {
                ordenado = false;
                intCmb(vVotos[i], vVotos[i + 1]);
            }
    } while (!ordenado);
}

void inicvVotos(tvVotos vVotos, ushort &cTemas) {
    cout << "Ingrese la cantidad de temas: ";
    cin >> cTemas;

    for (ushort i = 0; i < cTemas; i++) {
        cout << "Tema " << i + 1 << ": ";
        cin.ignore();
        cin.get(vVotos[i].nomTema, 21);
        vVotos[i].cantVotos = 0;
    }
}

void obtDatos(sEnc &rEnc) {
    cout << "\nSexo ('M', 'V', '*'): ";
    cin >> rEnc.sexo;

    if (rEnc.sexo == '*') {
        rEnc.edad = 0;
        strcpy(rEnc.nomTema, "*");
        return;
    }

    cout << "Edad (>= 15): ";
    cin >> rEnc.edad;
    cout << "Nombre Tema: ";
    cin.ignore();
    cin.get(rEnc.nomTema, 21);
}

void acumVotos(tvVotos vVotos, sEnc rEnc, ushort cTemas) {
    short pos = busBinVec(vVotos, rEnc.nomTema, cTemas);

    if (pos == -1)
        return;

    bool acumulador = false;

    if (rEnc.sexo == 'M' && rEnc.edad >= 30 && rEnc.edad <= 50)
        acumulador = true;

    if (rEnc.sexo == 'V' && (rEnc.edad < 30 || rEnc.edad > 50))
        acumulador = true;

    if (acumulador)
        vVotos[pos].cantVotos++;
}

void listadoTemasMusicales(tvVotos vVotos, ushort cTemas) {
    ordxBur(vVotos, cTemas);
    cout << left << setw(6) << "Puesto" << " " << setw(20) << "Tema" << " "
         << setw(5) << "Votos\n";

    for (ushort i = 0; i < cTemas; i++) {
        cout << left << setw(6) << i + 1 << " " << setw(20) << vVotos[i].nomTema
             << " " << setw(5) << vVotos[i].cantVotos << "\n";
    }
}

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
