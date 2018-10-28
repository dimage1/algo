#include <stdio.h>
#include <stack>

using namespace std;
 
//https://www.geeksforgeeks.org/reverse-stack-without-using-extra-space/
void reverse(stack<int> &sOld, stack<int> &sNew)
{
    while (sOld.size()) {
        int a = sOld.top();
        printf("%d ", a);
        sOld.pop();
        sNew.push(a);
    }
}

void test(stack<int> &stk) {
    stack<int> sNew;
    reverse(stk, sNew);
    printf(" >> ");
    while (sNew.size()) {
        int a = sNew.top();
        printf("%d ", a);
        sNew.pop();
    }
    printf("\n");
    
}

int main() {

    stack<int> s1;
    s1.push(1);
    s1.push(2);
    s1.push(3);

    test(s1);

    return 0;
}
