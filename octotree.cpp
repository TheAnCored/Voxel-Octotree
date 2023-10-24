#include "octotree.hpp"

//----------------POINT--------------------
Point::Point(double* tmp){
    for(int i=0; i<N; i++){
        this->coordinates_[i] = tmp[i];
    }
}

double& Point::operator[](int i){
    return this->coordinates_[i];
}
//-----------------------------------------

//----------------CUBE---------------------
Cube::Cube(unique_ptr<Point[]> tmp){
    this->vertexes_ = move(tmp);
}


Cube::Cube(double** tmp){

    this->vertexes_ = make_unique<Point[]>(8);

    for(int i=0; i < 8; i++){
        for(int j=0; j<N; j++){
            this->vertexes_[i][j] = tmp[i][j];
        }
    }
}


//-----------------------------------------