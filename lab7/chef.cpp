using namespace std;
#include <iostream>
#include <fstream>
#include <vector>

int maxSatisfaction(vector<int>& satisfaction) {

    int n = satisfaction.size(); 
    sort(satisfaction.begin(),satisfaction.end());      

    vector<int> su(n,0);  
    int cur_s = 0;
    su[n-1] = satisfaction[n-1]; //создаем массив частичных сумм (n*4 байт)
    for (int i = n-2; i >= 0; i--) { 
        su[i] = su[i+1] + satisfaction[i]; //заполняем массив с конца
    }
    int j = n - 1;
    while (j >= 0 && cur_s + su[j] > cur_s) {
        cur_s = cur_s + su[j]; //сравниваем текущую сумму, и сумму которую мы могли бы получит добавив еще один элемент
        j -= 1;
    }
    return cur_s;
}

int main() {
    vector<int> v(5,0);
    v[0] = -1;
    v[1] = -8;
    v[2] = 0;
    v[3] = 5;
    v[4] = -9;
    std:cout << maxSatisfaction(v1);
}