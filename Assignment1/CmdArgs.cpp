// Created by Mani Chandra Ganapathri on 08/01/24.
#include <iostream>
using namespace std;
// first
//
void print(const string& b) {
    cout << b << endl;
}
int main(int a, char** b) {
    for (int i = 0; i < a; i++) {
        if (b[i][0] != '-') {
            print(b[i]);
        }
    }
    return 0;
}