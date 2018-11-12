#include <stdio.h>

// https://practice.geeksforgeeks.org/problems/subsequence-matching
void checkMatch(const char *str) {
    const char *cur = str;
    while(cur && *cur) {
        if (*cur != 'R')
            goto err;
        cur++;    
        if (*cur == 0)
            break;
        else if (*cur == 'R')
            continue;
        else if (*cur != 'Y')
            goto err;
            
        cur++;
        if (*cur == 0)
            break;
        else if (*cur == 'R')
            continue;
        else if (*cur != 'Y')
            goto err;
        cur++;
    }
    
    printf("YES\n");
    return;
err:
    printf("NO\n");
}

int main() {

    checkMatch("R");
    checkMatch("RY");
    checkMatch("RWY");
    checkMatch("RYBY");
    checkMatch("RRYY");

    /*int t = 0;
    scanf("%d", &t);
    while (t--) {
        char *w;
        scanf("%s", w);
        checkMatch(w);
    }*/
    return 0;
}