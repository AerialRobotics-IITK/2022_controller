#include <iostream>
#include <math.h>
using namespace std;
/*

int main(){
    float V = 0; //initial start
    float Vo = 1; //reference value
    float a = 0.2; //linear increment constt
    float b = 3; //linear decay constt
    float time = 0;
    while(time < 5){
        if(V < Vo){
            float T = time; //timestamp at which we see it lower
            while (V < Vo + 0.5){
                V += a;
                time += 0.1;
                cout << V << endl;
            }
        }else{
            float T = time;
            float Vfix = V;
            while(V > Vo - 0.3){
                V = Vfix*(exp((T-time)*b));
                time += 0.1;
                cout << V << endl;
            }
        }
    }

}
*/

int main(){
    float V = 0; //initial start
    float Vo = 1; //reference value
    float a = 0.1; //linear increment constt
    float b = 3; //linear decay constt
    float time = 0;
    float fixTime = 0.5;
    while(time < 5){
        if(V <= Vo){
            while (V <= Vo){
                float T = time; //timestamp at which we see it lower
                while(time - T < fixTime){
                    time += 0.1;
                    V += a;
                    cout << V << ",\n";
                }
            }
        }else{
            float Vfix = V;
            while(V > Vo){
                float T = time;
                while(time - T < fixTime){
                    time += 0.1;
                    V = Vfix*(exp((T-time)*b));
                    cout << V << ",\n";
                }
            }
        }
    }

}