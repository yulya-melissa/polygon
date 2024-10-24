#include <iostream>
#include <vector>
using namespace std;


int main() {
    vector <int> vec = {2, 4, -1, -3, 9, -7, 1};
    int len_vec = vec.size();
    vector <int> ans;
    for (int i = 0; i < len_vec; i++) {
        for (int j = i; j < len_vec; j++) {
            for (int k = j; k < len_vec; k++) {
                for (int l = k; l < len_vec; l ++) {
                    if (vec[i] + vec[j] + vec[k] + vec[l] == 0) {
                        ans.push_back(i);
                        ans.push_back(j);
                        ans.push_back(k);
                        ans.push_back(l);
                    }
                }
            }
        }
    }
    int ans_arr[ans.size()][4];
    for (int i = 0; i < ans.size(); i++) {
        ans_arr[i/4][i%4] = ans[i];
    }
    cout << ans_arr;
    return 0;
}
