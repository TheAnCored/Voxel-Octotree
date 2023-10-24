#include <iostream>
#include <memory>

#define N 3

using namespace std;

class Point{
public:
    unique_ptr<double[]> coordinates_;

    Point(){ for(int i=0; i<N; i++){ coordinates_[i] = 0.; } }

    Point(double*);

    double& operator[](int);

    ~Point();
};


class Cube{
public:
    // Вершины куба
    unique_ptr<Point[]> vertexes_;  

    // Standart constructor
    Cube(){ this->vertexes_ = nullptr;};
    
    Cube(unique_ptr<Point[]>);
    Cube(double**);

    // Dividing the cube on eight equals by volume parts
    void dividing_the_cube();

    ~Cube();
};


class Link{

    unique_ptr<Cube> cube_;

    unique_ptr<Cube> descendant_1;
    unique_ptr<Cube> descendant_2;
    unique_ptr<Cube> descendant_3;
    unique_ptr<Cube> descendant_4;
    unique_ptr<Cube> descendant_5;
    unique_ptr<Cube> descendant_6;
    unique_ptr<Cube> descendant_7;
    unique_ptr<Cube> descendant_8;

};



class OrtoTree{

    // It is a point in Cartesian system of coordinate
    double point_;

    // It is a edge of a main(root) cube
    double edge_;

    int height_of_tree;

    unique_ptr<Link> Root;

    OrtoTree(){
        this->point_ = 0.; this->edge_ = 0.; height_of_tree = 0.; Root = nullptr;
    }

    OrtoTree(int);

    ~OrtoTree();
};