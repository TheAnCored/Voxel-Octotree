#include "header.hpp"

using namespace std;




class Point{
public:
    //-----------------------------------------------
    unique_ptr<double[]> coordinates_;

    //-----------------------------------------------
    // Пустой конструктор
    Point(){ for(int i=0; i<3; i++){ coordinates_[i] = 0.; } }

    Point(double, double, double);

    // Требует массив из даблов
    Point(double*);

    // Конструктор копирования
    Point(const Point&);

    // Оператор для взятия координаты
    double& operator[](int);

    // Оператор перемещения
    Point& operator=(Point&);

    Point& operator=(const Point&);

    // Comparsing two points
    bool operator==(Point);

    // Binary operations with two points
    Point& operator+(const Point&);
    Point& operator-(const Point&);

    // Operations with point and non-zero scalar
    Point& operator*(double);
    Point& operator/(double);

    // Расчёт дистанции между двумя точками
    double distance(Point);
};


class Cube{
public:
    // Vertexes of cube(it have only 8 points!)
    unique_ptr<Point[]> vertexes_; 

    // Центр куба
    Point center_;

    // Ребро куба
    double edge_;

    // Standart constructor
    Cube(){ 
        this->vertexes_ = make_unique<Point[]>(8); 
        for(int i = 0; i < 3; i++){this->center_[i] = 0.;}
    };

    // Конструктор перемещения
    Cube(Cube&&);
    
    Cube(Point, double);

    // Дан набор точек для куба как объекты Point
    Cube(unique_ptr<Point[]>);

    // Дан набор точек как матрица 3 на 8
    Cube(double**);

    // Оператор копирования
    Cube& operator=(const Cube&);

    // Внутри ли точка?
    bool IsInside(Point);
};


class Link{
public:
    // All have 8 descendants
    unique_ptr<Link[]> descendants_;

    // Cube than include points Убрать unique_ptr
    Cube cube_;

    // Level of tree
    unsigned int level_;

    // Пустой конструктор
    Link();

    // Конструктор перемещения
    Link(Link&&);

    // Конструктор
    Link(Cube&, unsigned int);

    Link(Point, double, unsigned int);

    // Оператор присваивания с перемещением
    Link& operator=(Link&&);

    // Function for division of cube
    void separation();
};

class Tree{
public:
    //
    unique_ptr<Link> root_;
    
    //
    unsigned int& amount_of_levels = N;

    // Empty constructor
    Tree(){ this->root_ = nullptr;}

    // 
    Tree(Point, double);
};