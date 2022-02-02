#include <iostream>
#include <map>
#include <array>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

struct Edge {

    Edge(string dest, string car) {
        this->car = move(car);
        this->dest = move(dest);
    }

    string dest;
    string car;
};

struct Q {

    Q(string dest, string car, string s) {
        this->car = move(car);
        this->dest = move(dest);
        this->s = move(s);
    }

    string dest;
    string car;
    string s;
};

int main() {

    cout << "Numarul de car neterminale:";
    int n;
    cin >> n;

    map<string, int> Vn;
    string car[n];

    for (int i = 0; i < n; ++i) {
        string s;
        cout << "Caracterul" << i << ": ";
        cin >> s;
        car[i] = s;
        Vn[s] = i;
    }

    string a[n][n + 1];

    vector<Edge> graph[n];


    fill(*a, *a + n * (n + 1), "-");


    cout << "P: " << endl;

    string s;
    getline(cin, s);

    while (s != "end") {
        getline(cin, s);
        string s1, s2, s3;
        int i1 = s.find('=');
        int i2 = s.find(' ');

        s1 = s.substr(0, i1);
        s2 = s.substr(i1 + 1, i2 - 1 - i1);
        s3 = s.substr(i2 + 1, s.length() - 1 - i2);


        if (s != "end") {
            if (((Vn[s1] == 0 and s1 != "S") or (Vn[s3] == 0 and s3 != "S")) and i2 != -1)
                cout << "Eroare! Caracter neterminabil gresit! Introdu inca odata!" << endl;
            else if (i2 == -1) {
                if (s2 == "empty") s2 = "";
                graph[Vn[s1]].emplace_back(Edge("SF", s2));
                if (s2.empty()) s2 = "*";
                if (a[Vn[s1]][n] == "-") a[Vn[s1]][n] = s2;
                else a[Vn[s1]][n] = a[Vn[s1]][n] + '|' + s2;
            } else {
                graph[Vn[s1]].emplace_back(Edge(s3, s2));
                if (a[Vn[s1]][Vn[s3]] == "-") a[Vn[s1]][Vn[s3]] = s2;
                else a[Vn[s1]][Vn[s3]] = a[Vn[s1]][Vn[s3]] + '|' + s2;
            }
        }
    }

    int w = 10;
    cout << setw(w) << "";
    for (int i = 0; i < n; ++i) {
        cout << setw(w) << "Q" << i;
    }
    cout << setw(w) << "QF";
    cout << endl;

    for (int i = 0; i < n; ++i) {
        cout << setw(w) << "Q" << i;
        for (int j = 0; j < n + 1; ++j) {
            cout << setw(w) << a[i][j];
        }

        cout << endl;
        cout << endl;
    }


    string to_check;
    cout << "Introdu string pentru verificare :";
    cin >> to_check;

    while (to_check != "end") {

        bool t = false;

        queue<Q> qu;
        for (auto &i: graph[0]) {
            qu.push(Q(i.dest, i.car, ""));
        }

        while (!qu.empty()) {
            string obt = qu.front().s + qu.front().car;

            if (qu.front().dest == "SF" and obt == to_check) {
                t = true;
            } else if (obt == to_check.substr(0, obt.length()) and qu.front().dest != "SF") {

                for (int i = 0; i < graph[Vn[qu.front().dest]].size(); ++i) {
                    qu.push(Q(graph[Vn[qu.front().dest]][i].dest, graph[Vn[qu.front().dest]][i].car, obt));
                }
            }


            qu.pop();
        }

        if (t) cout << "Este posibil de obtinut acest string!" << endl << endl;
        else
            cout << "NU este posibil de obtinut acest string!" << endl << endl;

        cout << "Introdu string pentru verificare :";
        cin >> to_check;
    }

    return 0;

}