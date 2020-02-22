#include <iostream>
#include "Picture.cpp"

using namespace std;

int main(int argc, char** argv){
    Picture picture(argv[1]);
    switch (argv[3][0]){
        case '0':
            picture.Inverse();
            break;
        case '1':
            picture.Mirror("H");
            break;
        case '2':
            picture.Mirror("V");
            break;
        case '3':
            picture.Rotation("R");
            break;
        case '4':
            picture.Rotation("L");
            break;
        default:
            cout << "you wrote wrong number"<< endl;
    }
    picture.Output(argv[2]);
    return 0;
}