
// Online C++ compiler to run C++ program online
#include <iostream>
using namespace std;
bool ray_casting(int point[2], int polygon[3][2]){
    bool is_in=false;
    int x=point[0];
    int y=point[1];
    int i=0;
    for(int i=0; i < 2; ++i){
        int x1=polygon[i][0];
        int x2=polygon[i+1][0];
        int y1=polygon[i][1];
        int y2=polygon[i+1][1];

        if(y < y1 != y < y2 && x < (x2-x1) * (y-y1) / (y2-y1) + x1){
            is_in=!is_in;
        }
    }

    return is_in;
}
int main() {
    // Write C++ code here
    std::cout << "Hello world!";
    int polygon [3][2] = { {100, 100}, {300, 100}, {300,300} };
    int point[2]={250,200};
    std::cout<<ray_casting(point,polygon);
    return 0;
}