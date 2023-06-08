#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

bool isRightTriangle(double a, double b, double c) {
    if (a*a + b*b == c*c || b*b + c*c == a*a || c*c + a*a == b*b) {
        return true;
    }
    return false;
}

bool isValidTriangle(double a, double b, double c) {
    if (a <= 0 || b <= 0 || c <= 0) {
        cout << "Ошибка: длины сторон должны быть положительными.\n";
        return false;
    }

    if (a + b <= c || b + c <= a || a + c <= b) {
        cout << "Ошибка: сумма длин двух сторон должна быть больше длины третьей стороны\n";
        return false;
    }
    return true;
}

double area(double a, double b, double c) {
    double p = (a + b + c) / 2;
    return sqrt(p * (p - a) * (p - b) * (p - c));
}

void input(vector<vector<double>>& triangles) {
    int n;
    cout << "Введите количество треугольников: ";
    cin >> n;

    if (cin.fail()) {
        cout << "Ошибка: это не число.\n";
        cin.clear(); // reset failbit
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
    }

    for (int i = 0; i < n; i++) {
        double a, b, c;
        cout << "Введите длины сторон треугольника " << i+1 << ": ";
        cin >> a >> b >> c;

        if (cin.fail()) {
            cout << "Ошибка: здесь нужно ввести 3 числа.\n";
            cin.clear(); // reset failbit
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //skip bad input
            i--; // повторяем ввод
            continue;
        }

        if (!isValidTriangle(a, b, c)) {
            i--; // повторяем ввод
            continue;
        }

        if (isRightTriangle(a, b, c)) {
            double currentArea = area(a, b, c);
            triangles.push_back({a, b, c, currentArea});
            double totalArea = 0;
            for (vector<double> triangle : triangles) {
                totalArea += triangle[3];
            }
            cout << "Текущая суммарная площадь прямоугольных треугольников: " << totalArea << endl;
        }
        else {
            triangles.push_back({a, b, c, 0});
        }
    }
}

void inputFromFile(vector<vector<double>>& triangles, const string& fileName) {
    ifstream inputFile;
    inputFile.open(fileName);

    if (!inputFile) {
        cerr << "Ошибка: не удалось открыть файл.\n";
        return;
    }

    double a, b, c;
    while(inputFile >> a >> b >> c ) {

        if (inputFile.fail()) {
            cout << "Ошибка: найдено не-числовое значение в файле, падаем"  << endl;
            exit(1);
        }

        if (!isValidTriangle(a, b, c)) {
            printf("Предупреждение: найден несуществующий треугольник %f %f %f - пропускаем его и идем дальше...\n", a, b, c);
            continue;
        }

        if (isRightTriangle(a, b, c)) {
            double currentArea = area(a, b, c);
            triangles.push_back({a, b, c, currentArea});
            double totalArea = 0;
            for (vector<double> triangle : triangles) {
                totalArea += triangle[3];
            }
            cout << "Текущая суммарная площадь прямоугольных треугольников: " << totalArea << endl;
        }
        else {
            triangles.push_back({a, b, c, 0});
        }
    }

    inputFile.close();
}

void randomInput(vector<vector<double>>& triangles, const int& count, const double& minLength, const double& maxLength) {
    for (int i = 0; i < count; i++) {
        double a = minLength + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (maxLength - minLength)));
        double b = minLength + static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (maxLength - minLength)));
        double c;
        if (rand() % 2 == 0) {
            c = minLength +
                static_cast<double>(rand()) / (static_cast<double>(RAND_MAX / (maxLength - minLength)));
        } else {
            c = sqrt(a*a + b*b);
        }

        if (isRightTriangle(a, b, c)) {
            double currentArea = area(a, b, c);
            triangles.push_back({a, b, c, currentArea});
            double totalArea = 0;
            for (vector<double> triangle : triangles) {
                totalArea += triangle[3];
            }
        }
        else {
            triangles.push_back({a, b, c, 0});
        }
    }
}

void output(vector<vector<double>> triangles) {
    double totalArea = 0;
    int rightTrianglesCount = 0;

    cout << "Данные о треугольниках:" << endl;
    for (int i = 0; i < triangles.size(); i++) {
        cout << "Треугольник " << i+1 << ": " << triangles[i][0] << " " << triangles[i][1] << " " << triangles[i][2];
        if (triangles[i][3] > 0) {
            cout << " (прямоугольный, площадь " << triangles[i][3] << ") ";
            totalArea += triangles[i][3];
            rightTrianglesCount++;
            printf("Текущая суммарная площадь прямоугольных треугольников: %f\nКол-во прямоугольных: %i",
                   totalArea,
                   rightTrianglesCount);
        }
        cout << endl;
    }

    printf("Итоговая суммарная площадь прямоугольных треугольников: %f\nКол-во прямоугольных: %i",
           totalArea,
           rightTrianglesCount);
}

int main()
{
    vector<vector<double>> triangles;

    char choice;
    cout << "Введите '1', чтобы ввести данные вручную, '2', чтобы ввести данные из файла, или '3', чтобы сгенерировать случайные данные: ";
    cin >> choice;

    if (choice == '1') {
        input(triangles);
    }
    else if (choice == '2') {
        string fileName;
        cout << "Введите путь к файлу: ";
        cin >> fileName;
        inputFromFile(triangles, fileName);
    }
    else if (choice == '3') {
        int n;
        double minLength, maxLength;
        cout << "Введите количество треугольников: ";
        cin >> n;
        cout << "Введите минимальную и максимальную длину стороны: ";
        cin >> minLength >> maxLength;
        if (minLength <= 0 || maxLength <= 0 || minLength > maxLength) {
            cerr << "Ошибка: некорректно введены данные.\n";
            return 0;
        }

        randomInput(triangles, n, minLength, maxLength);
    }
    else {
        cerr << "Ошибка: некорректный ввод.\n";
        return 0;
    }

    output(triangles);

    return 0;
}
