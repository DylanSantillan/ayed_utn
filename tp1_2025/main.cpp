#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

typedef char str30[31];
typedef char str20[21];
typedef char str10[11];
typedef unsigned short ushort;

const ushort MAX_ART = 10000;
const ushort MAX_CPRA = 100;
const ushort CANT_RUB = 15;
const ushort OFERTAS = 7;
const ushort TAM_LINEA = 104;

struct sArt {
    int codArt;
    short codRub;
    str30 descArt;
    ushort stock;
    float preUni;
    str10 uniMed;
    short ofertas[OFERTAS * 2];
};

struct sDescArt {
    str30 descArt;
    int pos;
    bool estado;
};

struct sRub {
    short codRub;
    str20 descRub;
};

struct sCpra {
    str30 descArt;
    short cantReq;
};

struct sRubArt {
    short codRub;
    int pos;
};

typedef sDescArt tvrIndArt[MAX_ART];
typedef sRub tvrRub[CANT_RUB];
typedef sCpra tvrLstCpra[MAX_CPRA];
typedef sRubArt tvrRubArt[MAX_ART];

void Abrir(fstream &Art, ifstream &IndArt, ifstream &Rub, ifstream &LstCpra) {
    Art.open("Articulos.txt");
    IndArt.open("IndDescripArt.txt");
    Rub.open("Rubros.txt");
    LstCpra.open("ListaCompras.txt");
}

void IntCmb(sRubArt &elem1, sRubArt &elem2) {
    sRubArt aux = elem1;
    elem1 = elem2;
    elem2 = aux;
}

void OrdxBur(tvrRubArt &vrRubArt, ushort cantArt) {
    ushort k = 0;
    bool ordenado;

    do {
        ordenado = true;
        k++;

        for (ushort i = 0; i < cantArt - k; i++)
            if (vrRubArt[i].codRub > vrRubArt[i + 1].codRub) {
                ordenado = false;
                IntCmb(vrRubArt[i], vrRubArt[i + 1]);
            }

    } while (!ordenado);
}

bool LeerArt(fstream &Art, sArt &rArt) {
    Art >> rArt.codArt >> rArt.codRub;
    Art.ignore();
    Art.get(rArt.descArt, 31);
    Art >> rArt.stock >> rArt.preUni;
    Art.ignore();
    Art.get(rArt.uniMed, 11);

    for (ushort i = 0; i < OFERTAS * 2; i++)
        Art >> rArt.ofertas[i];

    Art.ignore();
    return Art.good();
}

bool LeerIndArt(ifstream &IndArt, sDescArt &rDescArt) {
    IndArt.get(rDescArt.descArt, 31);
    IndArt >> rDescArt.pos >> rDescArt.estado;
    IndArt.ignore();
    return IndArt.good();
}

bool LeerRub(ifstream &Rub, sRub &rRub) {
    Rub >> rRub.codRub;
    Rub.ignore();
    Rub.get(rRub.descRub, 21);
    Rub.ignore();
    return Rub.good();
}

bool LeerLstCpra(ifstream &LstCpra, sCpra &rCpra) {
    LstCpra.get(rCpra.descArt, 31);
    LstCpra >> rCpra.cantReq;
    LstCpra.ignore();
    return LstCpra.good();
}

void VolcarArchivos(fstream &Art, ifstream &IndArt, tvrIndArt &vrIndArt,
                    ifstream &Rub, tvrRub &vrRub, ifstream &LstCpra,
                    tvrLstCpra &vrLstCpra, tvrRubArt &vrRubArt, ushort &cantArt,
                    ushort &cantCpra) {
    sArt rArt;
    sDescArt rDescArt;
    sRub rRub;
    sCpra rCpra;

    while (LeerIndArt(IndArt, rDescArt) && cantArt < MAX_ART)
        vrIndArt[cantArt++] = rDescArt;

    while (LeerLstCpra(LstCpra, rCpra) && cantCpra < MAX_CPRA)
        vrLstCpra[cantCpra++] = rCpra;

    for (ushort i = 0; LeerRub(Rub, rRub) && i < CANT_RUB; i++)
        vrRub[i] = rRub;

    for (ushort i = 0; LeerArt(Art, rArt) && i < cantArt; i++) {
        vrRubArt[i].codRub = rArt.codRub;
        vrRubArt[i].pos = i;
    }

    OrdxBur(vrRubArt, cantArt);
}

int BusBinVec(tvrIndArt vrIndArt, str30 clave, ushort cantArt) {
    int ult = cantArt - 1;
    int prim = 0;
    int med;

    while (prim <= ult) {
        med = (prim + ult) / 2;

        if (strcmp(vrIndArt[med].descArt, clave) == 0)
            return med;
        else if (strcmp(vrIndArt[med].descArt, clave) < 0)
            prim = med + 1;
        else
            ult = med - 1;
    }

    return -1;
}

void ActLinea(fstream &Art, sArt &rArt, ushort posArt) {
    Art.clear();
    Art.seekp(TAM_LINEA * posArt);

    Art << setw(8) << rArt.codArt << " " << setw(2) << rArt.codRub << " "
        << setw(30) << rArt.descArt << " " << setw(4) << rArt.stock << " "
        << setw(9) << setprecision(2) << fixed << rArt.preUni << " " << setw(10)
        << rArt.uniMed;

    for (ushort i = 0; i < OFERTAS; i++) {
        Art << " " << setw(1) << rArt.ofertas[i * 2] << " " << setw(2)
            << rArt.ofertas[i * 2 + 1];
    }

    Art << "\n";
}

void ProcCompras(fstream &Art, tvrIndArt vrIndArt, tvrLstCpra &vrLstCpra,
                 ushort cantArt, ushort cantCpra) {
    sArt rArt;

    for (ushort i = 0; i < cantCpra; i++) {
        short posInd = BusBinVec(vrIndArt, vrLstCpra[i].descArt, cantArt);

        if (posInd == -1 || !vrIndArt[posInd].estado) {
            vrLstCpra[i].cantReq = 0;
            continue;
        }

        ushort posArt = vrIndArt[posInd].pos;
        Art.clear();
        Art.seekg(TAM_LINEA * posArt);
        LeerArt(Art, rArt);

        if (rArt.stock >= vrLstCpra[i].cantReq) {
            rArt.stock -= vrLstCpra[i].cantReq;

        } else {
            vrLstCpra[i].cantReq = rArt.stock;
            rArt.stock = 0;
        }

        ActLinea(Art, rArt, posArt);
    }
}

long GetDate(int &anio, int &mes, int &dia, int &ds) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    anio = 1900 + timeinfo->tm_year;
    mes = 1 + timeinfo->tm_mon;
    dia = timeinfo->tm_mday;
    ds = 1 + timeinfo->tm_wday;
    return (1900 + timeinfo->tm_year) * 10000 + (1 + timeinfo->tm_mon) * 100 +
           timeinfo->tm_mday;
}

long GetTime(int &hh, int &mm, int &ss) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    hh = timeinfo->tm_hour;
    mm = timeinfo->tm_min;
    ss = timeinfo->tm_sec;
    return timeinfo->tm_hour * 10000 + timeinfo->tm_min * 100 +
           timeinfo->tm_sec;
}

string Replicate(char car, unsigned n) {
    string cad = "";
    for (unsigned i = 1; i <= n; i++)
        cad = cad + car;
    return cad;
}

void CabeceraTicket(int &ds) {
    int anio, mes, dia, hh, mm, ss;
    str10 nomDia;

    GetDate(anio, mes, dia, ds);
    GetTime(hh, mm, ss);

    switch (ds) {
        case 1:
            strcpy(nomDia, "Domingo");
            break;
        case 2:
            strcpy(nomDia, "Lunes");
            break;
        case 3:
            strcpy(nomDia, "Martes");
            break;
        case 4:
            strcpy(nomDia, "Miercoles");
            break;
        case 5:
            strcpy(nomDia, "Jueves");
            break;
        case 6:
            strcpy(nomDia, "Viernes");
            break;
        default:
            strcpy(nomDia, "Sabado");
            break;
    }

    cout << "K O T T O\n"
         << "Yo te reconozco\n"
         << "SUC 170\n"
         << "XXXXXXX 9999\n"
         << "XXXX\n"
         << "C.U.I.T. 99-99999999-9\n"
         << "Fecha: " << nomDia << " " << setw(2) << dia << "/" << setw(2)
         << mes << "/" << setw(4) << anio << "\nhora: " << setw(2) << hh << ":"
         << setw(2) << mm << ":" << setw(2) << ss << "\n"
         << "Nro. Ticket: 9999-99999999\n"
         << "Nro. Caja: 9999\n"
         << Replicate('-', 41) << "\n"
         << "F A C T U R A - B\n"
         << Replicate('-', 41) << "\n";
}

void PieTicket(float impTot, float impTotDesto, float impTotConDesto) {
    cout << "Su pago con Tipo Pago: " << impTotConDesto << "\n"
         << "Su vuelto: 0\n"
         << " G R A C I A S  P O R  S U  C O M P R A\n"
         << " Para consultas, sugerencias o reclamos\n"
         << " comunicarse al correo infoKotto.com.ar\n";
}

void EmitirTicket(fstream &Art, tvrIndArt &vrIndArt, tvrLstCpra &vrLstCpra,
                  ushort cantArt, ushort cantCpra) {
    sArt rArt;
    int ds;
    str10 promoDescto;
    float impTot = 0.0f, impTotDesto = 0.0f;

    freopen("Ticket.txt", "w", stdout);
    CabeceraTicket(ds);

    for (ushort i = 0; i < cantCpra; i++) {
        short posInd = BusBinVec(vrIndArt, vrLstCpra[i].descArt, cantArt);

        if (posInd == -1 || vrLstCpra[i].cantReq == 0)
            continue;

        ushort posArt = vrIndArt[posInd].pos;
        Art.clear();
        Art.seekg(TAM_LINEA * posArt);
        LeerArt(Art, rArt);

        ushort tipoDescto = rArt.ofertas[(ds - 1) * 2];
        ushort porcDescto = rArt.ofertas[(ds - 1) * 2 + 1];

        float impTotItem = rArt.preUni * vrLstCpra[i].cantReq;
        float impDescto = impTotItem * porcDescto / 100.0f;

        switch (tipoDescto) {
            case 1:
                strcpy(promoDescto, "Promo");
                break;
            case 2:
                strcpy(promoDescto, "Marca");
                break;
            case 3:
                strcpy(promoDescto, "Jub.");
                break;
            case 4:
                strcpy(promoDescto, "Comu.");
                break;
            case 5:
                strcpy(promoDescto, "MercPago");
                break;
            case 6:
                strcpy(promoDescto, "ANSES");
                break;
            default:
                strcpy(promoDescto, "SinPromo");
                break;
        }

        cout << setw(2) << vrLstCpra[i].cantReq << " x " << "$ " << setw(9)
             << rArt.preUni << "\n"
             << setw(30) << rArt.descArt << " " << setw(10) << rArt.uniMed
             << "\n"
             << setw(8) << rArt.codArt << setw(21) << " " << "$ " << right
             << setw(10) << impTotItem << "\n";

        if (strcmp(promoDescto, "SinPromo") != 0) {
            cout << left << setw(10) << promoDescto << " " << tipoDescto
                 << setw(17) << " " << "$ " << right << setw(10) << impDescto
                 << "\n";

            impTotDesto += impDescto;
        }

        impTot += impTotItem;
        cout << "\n";
    }

    float impTotConDesto = impTot - impTotDesto;

    cout << Replicate('=', 41) << "\n"
         << "T O T A L" << setw(19) << " " << "$ " << setprecision(2) << fixed
         << right << setw(11) << impTotConDesto << "\n"
         << Replicate('=', 41) << "\n\n";

    PieTicket(impTot, impTotDesto, impTotConDesto);
    fclose(stdout);
}

void EmitirArt_x_Rubro(fstream &Art, tvrRub &vrRub, tvrRubArt &vrRubArt,
                       ushort cantArt) {
    sArt rArt;
    short posRub = -1;

    freopen("Ticket.txt", "a", stdout);

    cout << "\n\n" << Replicate('-', TAM_LINEA) << "\n";
    cout << "Listado de Articulos ordenados por Código de Rubro\n";
    cout << Replicate('=', TAM_LINEA) << "\n";

    for (ushort i = 0; i < cantArt; i++) {
        if (vrRubArt[i].codRub != vrRub[posRub].codRub) {
            posRub++;
            cout << "\nCod. Rubro: " << vrRubArt[i].codRub << " - "
                 << vrRub[posRub].descRub << "\n";
            cout << "Cod.Art. Descripcion                    Stk.  Pre.Uni. "
                    "U.Med.     TD % TD % TD % TD % TD % TD % TD %"
                 << "\n";
            cout << Replicate('-', TAM_LINEA) << "\n";
        }

        Art.clear();
        Art.seekg(TAM_LINEA * vrRubArt[i].pos);
        LeerArt(Art, rArt);

        cout << setw(8) << rArt.codArt << " " << setw(30) << rArt.descArt << " "
             << setw(4) << rArt.stock << " " << setw(9) << rArt.preUni << " "
             << setw(10) << rArt.uniMed;

        for (ushort i = 0; i < OFERTAS; i++) {
            cout << " " << setw(1) << rArt.ofertas[i * 2] << " " << setw(2)
                 << rArt.ofertas[i * 2 + 1];
        }

        cout << "\n";
    }

    fclose(stdout);
}

void Cerrar(fstream &Art, ifstream &IndArt, ifstream &Rub, ifstream &LstCpra) {
    Art.close();
    IndArt.close();
    Rub.close();
    LstCpra.close();
}

int main() {
    tvrIndArt vrIndArt;
    tvrRub vrRub;
    tvrLstCpra vrLstCpra;
    tvrRubArt vrRubArt;

    fstream Art;
    ifstream IndArt, Rub, LstCpra;

    ushort cantArt = 0, cantCpra = 0;

    Abrir(Art, IndArt, Rub, LstCpra);
    VolcarArchivos(Art, IndArt, vrIndArt, Rub, vrRub, LstCpra, vrLstCpra,
                   vrRubArt, cantArt, cantCpra);
    ProcCompras(Art, vrIndArt, vrLstCpra, cantArt, cantCpra);
    EmitirTicket(Art, vrIndArt, vrLstCpra, cantArt, cantCpra);
    EmitirArt_x_Rubro(Art, vrRub, vrRubArt, cantArt);
    Cerrar(Art, IndArt, Rub, LstCpra);
    return 0;
}