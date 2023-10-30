#include "octotree.hpp"


//----------------POINT--------------------
Point::Point(double x, double y, double z){
    this->coordinates_[0] = x;
    this->coordinates_[1] = y;
    this->coordinates_[2] = z;
}

Point::Point(double* tmp){
    for(int i=0; i<3; i++){
        this->coordinates_[i] = tmp[i];
    }
}

Point::Point(const Point& second){
    for(int i=0; i<3; i++){
        this->coordinates_[i] = second.coordinates_[i];
    }
}

Point& Point::operator=(const Point& second){
    for(int i=0; i<3; i++){
        this->coordinates_[i] = second.coordinates_[i];
    }

    return *this;
}

double& Point::operator[](int i){
    return this->coordinates_[i];
}

Point& Point::operator=(Point& second){
    this->coordinates_ = move(second.coordinates_);

    return *this;
}

bool Point::operator==(Point second){

    int index = 0;

    for(int i=0; i<3; i++){
        if(abs(this->coordinates_[i] - second.coordinates_[i]) < 
        numeric_limits<double>::epsilon()*max(abs(this->coordinates_[i]),abs(second.coordinates_[i])))
        {
            index++;
        }
    }

    if(index!=3){ return 0;}

    return 1;
}

Point& Point::operator+(const Point& second){
    for(int i=0; i<3; i++){
        this->coordinates_[i] += second.coordinates_[i];
    }

    return *this;
}

Point& Point::operator-(const Point& second){
    for(int i=0; i<3; i++){
        this->coordinates_[i] -= second.coordinates_[i];
    }

    return *this;
}

Point& Point::operator*(double lambda){

    for(int i=0; i<3; i++){
        this->coordinates_[i] *= lambda;
    }

    return *this;
}

Point& Point::operator/(double lambda){
    if(lambda == 0){ throw "Division by zero!\n"; return *this;}

    for(int i=0; i<3; i++){
        this->coordinates_[i] /= lambda;
    }

    return *this;
}

double Point::distance(Point second){

    double distance = 0.;

    Point tmp;

    tmp = *this - second;

    for(int i=0; i<3; i++){
        distance += pow(tmp.coordinates_[i], 2);
    }

    distance = pow(distance, 1/2);

    return distance;
}
//-----------------------------------------

//----------------CUBE---------------------
Cube::Cube(unique_ptr<Point[]> tmp){
    this->vertexes_ = move(tmp);

    Point TMP;
    double sum = 0.;

    for(int i=0; i<3; i++){
        for(int j = 0; j<8; j++){
            double sum=0.;

            sum += this->vertexes_[j][i];

            if(j == 7){
                this->center_[i] = sum/8;

                sum =0.;
            }
        }
    }

    TMP = this->center_ - this->vertexes_[0];

    for(int i=0; i<3; i++){
        sum += pow(TMP.coordinates_[i], 2);
    }

    this->edge_ = pow(sum, 1/2);
}


Cube::Cube(double** tmp){

    double sum=0.;
    Point TMP;

    this->vertexes_ = make_unique<Point[]>(8);

    for(int i=0; i < 8; i++){
        for(int j=0; j<3; j++){
            this->vertexes_[i][j] = tmp[i][j];
        }
    }

    for(int i=0; i<3; i++){
        for(int j = 0; j<8; j++){

            sum += this->vertexes_[j][i];

            if(j == 7){
                this->center_[i] = sum/8;

                sum = 0.;
            }
        }
    }

    TMP = this->center_ - this->vertexes_[0];

    for(int i=0; i<3; i++){
        sum += pow(TMP.coordinates_[i], 2);
    }

    this->edge_ = pow(sum, 1/2);
}

Cube::Cube(Point center, double edge){
    this->center_ = center;
    this->edge_ = edge;

    unique_ptr<Point[]> TMP = make_unique<Point[]>(8);

    for(int i=0; i<3; i++){
        TMP[0][i] = center[i] + edge/2; 
        TMP[7][i] = center[i] - edge/2;
    }

    for(int i=0; i<3; i++){
        TMP[i+1] = TMP[0];
        TMP[i+4] = TMP[7];

        TMP[i+1][i] = TMP[0][i] + edge;
        TMP[i+4][i] = TMP[7][i] - edge;
    }
}

Cube& Cube::operator=(const Cube& second){

    for(int i=0; i<3; i++){
        this->vertexes_[i] = second.vertexes_[i];
    }
    this->center_ = second.center_;
    this->edge_ = second.edge_;

    return *this;
}

bool Cube::IsInside(Point point){

    int index=0;

    Point min, max;
    double sum_cur=0.;

    double sum=0.;

    for(int i=0; i<8; i++){
        if(i == 0){ min = this->vertexes_[i];}

        for(int j=0; j<3; j++){
            sum += this->vertexes_[i].coordinates_[j];
        
            if(i == 0){
                sum_cur = sum;
                sum = 0.;
            }
        }

        if(i>0 && abs(sum_cur) < abs(sum)){
            min = this->vertexes_[i];
            sum_cur = sum;
            sum = 0.;
        }
    }

    sum_cur = 0.;
    sum = 0.;

    for(int i=0; i<8; i++){
        if(i == 0){ max = this->vertexes_[i];}

        for(int j=0; j<3; j++){
            sum += this->vertexes_[i].coordinates_[j];
        
            if(i == 0){
                sum_cur = sum;
                sum = 0.;
            }
        }

        if(i>0 && abs(sum_cur) > abs(sum)){
            max = this->vertexes_[i];
            sum_cur = sum;
            sum = 0.;
        }
    }

    for(int i=0; i<3; i++){
        if(min.coordinates_[i] <= point.coordinates_[i] && max.coordinates_[i] >= point.coordinates_[i]){
            index++;
        }
    }

    if(index == 3){ return 1;}

    return 0;
}
//-----------------------------------------

//----------------LINK---------------------

Link::Link(){
    this->cube_ = Cube(); 
    this->descendants_ = nullptr; 
    this->level_ = 1;
}

Link::Link(Point center, double edge, unsigned int level){
    
    this->cube_ = Cube(center, edge);
    this->descendants_ = nullptr;
    this->level_ = level;

    if(level != N-1){ this->separation(); }
}

Link::Link(Cube& tmp_cube, unsigned int level){
    this->descendants_ = nullptr;
    this->cube_ = tmp_cube;
    this->level_ = level;

    if(level != N-1){ this->separation(); }
}

// Moving constructor
Link::Link(Link&& second_link){
    this->descendants_ = move(second_link.descendants_);
    this->cube_ = move(second_link.cube_);
    this->level_ = second_link.level_;
}

Link& Link::operator=(Link&& second){
    this->descendants_ = move(second.descendants_);
    this->cube_ = move(second.cube_);

    return *this;
}

// Function for division of cube
void Link::separation(){

    double segment_edge = this->cube_.edge_/2;
    double segment_diag = this->cube_.edge_* sqrt(2)*(1/2);

    for(int i=0; i<8; i++){

        unique_ptr<Point[]> TMP = make_unique<Point[]>(8);
        Point current = this->cube_.vertexes_[i];

        TMP[0] = current;
        TMP[7] = this->cube_.center_;

        for(int j = 1; j < 7; j++){
            
            if(j<4){
                current.coordinates_[j-1] += segment_edge;

                if(this->cube_.IsInside(current)){
                    TMP[j] = current;
                }
                else{
                    current.coordinates_[j-1] -= 2*segment_edge;

                    TMP[j] = current;
                }
            }
            else{
                current.coordinates_[j-4] += segment_diag;

                if(this->cube_.IsInside(current)){
                    TMP[j] = current;
                }
                else{
                    current.coordinates_[j-4] -= 2*segment_diag;

                    TMP[j] = current;
                }
            }
        }

        Cube tmp_cube(move(TMP));
        Link tmp_link(tmp_cube, this->level_++);

        this->descendants_[i] = move(tmp_link);
    }
}
//-----------------------------------------


//----------------TREE---------------------

Tree::Tree(Point center, double edge){
    this->root_ = make_unique<Link>(center, edge, 1);
}