#include <iostream>
#include <cmath>
#include <cstdlib>

using namespace std;

int main(){

    // operator OU

    cout << "topology: 2 4 1"<< endl;
    for(int i=500;i>=0;i--){
        int n1 = (int)(2.0*rand()/double(RAND_MAX));
        int n2 = (int)(2.0*rand()/double(RAND_MAX));
        // int t = n1^n2; // XOR
        // int t = n1|n2; // ET
        int t = n1|n2;
        cout <<"in: "<<n1<<".0 "<<n2<<".0"<< endl;
        cout <<"out: " <<t<<".0 "<<endl;
    }

    return 0;

}