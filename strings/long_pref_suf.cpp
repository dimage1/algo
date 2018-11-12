#include <iostream>
#include <string.h>
using namespace std;

//https://practice.geeksforgeeks.org/problems/longest-prefix-suffix
//https://www.geeksforgeeks.org/kmp-algorithm-for-pattern-searching/
size_t longPref(const string &str) {
    size_t res = 0;
    
    string ss = str;
    char *cstr = (char*)str.c_str(); // for strstr :(
    const char *css = ss.c_str();
    
    size_t check = str.size() - 1;
    for(; check > 0; check--) {
        
        ss[check] = 0;
        //printf("check: %lu, suf: %p, pref: %s\n", check, suf, pref.c_str());
        if (strncmp(cstr + str.size() - check, css, check) == 0) {
            res = check;
            break;
        }
        ss[check] = str[check];
    }
    return res;
}

int main() {

    /*int t;
    cin >> t;
    while(t--) {
        string s;
        cin >> s;
        cout << longPref(s) << "\n";   
        
    }*/

    cout << longPref("aca") << "\n";
    cout << longPref("xxxxrxxxx") << "\n";
    cout << longPref("wrwbwrqwhwrwbw") << "\n";
    cout << longPref("azwkhazwdazwkhaqazwkhazwdacazwkhazwdazwkhaqazwkhaxazwkha"
                     "zwdazwkhaqazwkhazwdacazwkhazw") << "\n";

    return 0;
}