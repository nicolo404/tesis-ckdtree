#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <random>
#include <unordered_set>
#include <fstream>
#include <climits>
#include <sys/time.h>
#include <sstream>
#include <string>

using namespace std;

// Definición de la estructura Point para representar un punto en 2D
struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
};

// Definición de la estructura Point 3D

struct Point3D {
    int x, y, z;
    Point3D() : x(0), y(0), z(0) {}
    Point3D(int x, int y, int z) : x(x), y(y), z(z) {}
};

// Definición de la estructura Node para el árbol KD
struct Node {
    vector<int> point;
    int value;
    Node *left, *right;
    Node(vector<int> point) : point(point), left(nullptr), right(nullptr) {}
};
struct Rectangle {
    Point bottom_left;
    Point top_right;
    Rectangle() : bottom_left(0, 0), top_right(0, 0) {}
};

    class KdTree {
    private:
    Node *root;
    int k;


Node *buildTree(vector<vector<int>> &points, int start, int end, int depth) {
    if (start > end) return nullptr;
    int axis = depth % k;
    int mid = (start + end) / 2;
    nth_element(points.begin() + start, points.begin() + mid, points.begin() + end + 1,
            [axis, this](const vector<int>& a, const vector<int>& b) { 
                if (a[axis] == b[axis]) {
                    // Si los puntos tienen el mismo valor en el eje actual, ordenamos por el eje alternativo
                    return a[(axis + 1) % this->k] < b[(axis + 1) % this->k];
                }
                return a[axis] < b[axis]; 
            });


    Node *node = new Node(points[mid]);
    node->left = buildTree(points, start, mid - 1, depth + 1);
    node->right = buildTree(points, mid + 1, end, depth + 1);
    return node;
}
void generateCodes(Node *node, Point3D parent) {
        if (node == nullptr) return;
        Point3D current(node->point[0], node->point[1], node->point[2]);
        node->value = Spiral3d_code(current, parent);
        generateCodes(node->left, current);
        generateCodes(node->right, current);
}

public:
    KdTree(vector<vector<int>> points) {
        k = points[0].size();
        root = buildTree(points, 0, points.size() - 1, 0);
    }

    Node *getRoot() {
        return root;
    }

    vector<int> getRootPoint() {
        return root->point;
    }
    int size(Node *node) {
        if (node == nullptr) return 0;
        return 1 + size(node->left) + size(node->right);
    }
    int size() {
        return size(root);
    }

    void generarCodigos() {
       Point3D origin(root->point[0], root->point[1], root->point[2]);
       generateCodes(root, origin);
    }
    
   void printCodes(Node *node) {
       if (node == nullptr) return;
       printCodes(node->left);
       cout << "[";
       for(int i=0; i<node->point.size(); i++){
           cout << node->point[i];
           if(i != node->point.size()-1){
               cout << ", ";
           }  
       }
       cout << "] -> " << node->value << endl;
       printCodes(node->right);
   }

   unsigned int cara(unsigned int x,unsigned int y,unsigned int a){
	//esto calcular el numero a sumar dada la cara
	if(x%2==0) return (x+1)*a-y-1;
	else return x*a+y;
}

unsigned int Spiral3d_code(Point3D current, Point3D previous){
	//calcular diferencias
	int x = current.x-previous.x;
	int y = current.y-previous.y;
	int z = current.z-previous.z;
	//encontrar la capa
	unsigned int m;
	if(abs(x)>abs(y)) m = abs(x);
	else m=abs(y);
	if(abs(z)>m) m = abs(z);
	//calcular el numero de comienzo de la diagonal
	unsigned int L = pow((2*m - 1),3);
	//calcular el ancho de la cara
	unsigned int a = 2*m+1;
	//base de salida
	unsigned int salida = L;
	//calculamos lo que se suma por cara lateral
	unsigned int sumcara = a*(a-1);
	/*ver que cara estamos. Las caras serán numeradas desde
	z+, en orden derecha. Siempre la primera línea 
	vertical será numerada por la cara anterior.*/
	//cara 1 es z+, se decide arbitrariamente ir en -x
	if(m==z) return salida + cara(-x+m,y+m,a);
	//se debe sumar en lo que termina la cara anterior
	salida += sumcara;
	//cara 2 es x-, debe ir en direccion -z
	if(m==-x) return salida + cara(-z+m,y+m,a);
	//se debe sumar en lo que termina la cara anterior
	salida += sumcara;
	//cara 3 es z-, debe ir en direccion +x
	if(m==-z) return salida + cara(x+m,y+m,a);
	//se debe sumar en lo que termina la cara anterior
	salida += sumcara;
	//cara 4 es x, debe ir en direccion z
	if(m==x) return salida + cara(z+m,y+m,a);
	
	/*Caras restantes son especiales. Ambas trabajan 
	sin las orillas*/
	//sumamos lo que alcanzo a sumar la cara anterior
	salida += sumcara;
	//cara 5 es -y, debemos recalcular sus valores
	unsigned int newm = m-1;
	unsigned int newa = 2*newm+1;
	if(m==-y) return salida + cara(-x+newm,z+newm,newa);
	//se debe sumar en la cara anterior
	salida += newa*newa;
	//cara 6 es y
	if(m==y) return salida + cara(x+newm,z+newm,newa);
    return 0;
}

uint code(Point current, Point previous) {
    int x = current.x - previous.x;
    int y = current.y - previous.y;
    if (x == 0 && y == 0) {
        return 0;
    }
    uint max;  
    if (abs(x) >= abs(y)) max = abs(x); 
    else max = abs(y);
    uint length = (max << 1) + 1;
    uint initialPosition = (length - 2) * (length - 2);
    if (x == max) { 
        return initialPosition + max - y;
    } else if (x == -max) { 
        return initialPosition + max + 2 * (length - 1) + y;
    } else if (y == max) { 
        return initialPosition + max + 3 * (length - 1) + x;
    } else { 
        return initialPosition + max + (length - 1) - x;
    }
}
void getCodes(Node *node, vector<uint> &codes) {
        if (node == nullptr) return;
        getCodes(node->left, codes);
        codes.push_back(node->value);
        getCodes(node->right, codes);
}

};
Point decode(uint code, Point parent){
    if(code == 0){
        return parent;
    }
    uint sqr = (uint) floor(sqrt(code));
    uint length = (sqr % 2) + 1 + sqr;
    uint max = (length - 1) >> 1;
    uint initialPosition = (length-2) * (length-2);
    uint diff = code - initialPosition;
    uint zone = diff / (length-1);
    if(zone == 0){ 
        return Point(parent.x + max, parent.y + max - diff);
     }else if(zone == 1){ 
        return Point(parent.x + (length -1)+ max - diff, parent.y - max);
     }else if(zone == 2){ 
        return Point(parent.x - max, parent.y + diff - max - 2*(length -1));
     }else {
        return Point(parent.x + diff - max - 3*(length -1), parent.y + max);
     }
}
void kdTreeToArray(Node *node, vector<vector<int>> &points, int start, int end) {
    if (node == nullptr) return;
    int mid = (start + end) / 2;
    points[mid] = node->point;
    kdTreeToArray(node->left, points, start, mid - 1);
    kdTreeToArray(node->right, points, mid + 1, end);
}
void codigosToArray(Node *node, vector<uint> &codes, int start, int end) {
    if (node == nullptr) return;
    int mid = (start + end) / 2;
    codes[mid] = node->value;
    codigosToArray(node->left, codes, start, mid - 1);
    codigosToArray(node->right, codes, mid + 1, end);
}
void decodeArray(vector<uint> &codes, vector<Point> &decodedPoints, Point parent, int start, int end) {
    if (start > end) return;
    int mid = (start + end) / 2;
    Point current = decode(codes[mid], parent);
    decodedPoints[mid] = current;
    decodeArray(codes, decodedPoints, current, start, mid - 1);
    decodeArray(codes, decodedPoints, current, mid + 1, end);
}
void printDecodedPoints(vector<Point> &decodedPoints) {
    cout << "[";
    for(int i=0; i<decodedPoints.size(); i++){
        cout << "[" << decodedPoints[i].x << ", " << decodedPoints[i].y << "]";
        if(i != decodedPoints.size()-1){
            cout << ", ";
        }  
    }
    cout << "]" << endl;
}


void printVector(vector<vector<int>> &points) {
    cout << "[";
    for (int i = 0; i < points.size(); i++) {
        cout << "[";
        for (int j = 0; j < points[i].size(); j++) {
            cout << points[i][j];
            if (j != points[i].size() - 1) {
                cout << ", ";
            }
        }
        cout << "]";
        if (i != points.size() - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;

}
void imprimirCodigosDeArreglo(vector<uint> &codes) {
    cout << "[";
    for(int i=0; i<codes.size(); i++){
        cout << codes[i];
        if(i != codes.size()-1){
            cout << ", ";
        }  
    }
    cout << "]" << endl;
}
vector<vector<int>> generateRandomPoints(int numPoints, int maxCoordinate) {
    vector<vector<int>> points;
    unordered_set<string> pointSet;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, maxCoordinate);
    while (points.size() < numPoints) {
        int x = dis(gen);
        int y = dis(gen);
        string pointString = to_string(x) + "," + to_string(y);
        if (pointSet.find(pointString) == pointSet.end()) {
            points.push_back({x, y});
            pointSet.insert(pointString);
        }
    }
    return points;
}
// Función para generar puntos aleatorios en 3D
vector<vector<int>> generateRandomPoints3D(int numPoints, int maxCoordinate) {
    vector<vector<int>> points;
    unordered_set<string> pointSet; // Para evitar duplicados
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, maxCoordinate);
    
    while (points.size() < numPoints) {
        int x = dis(gen);
        int y = dis(gen);
        int z = dis(gen);
        
        // Convertir el punto a string para verificar duplicados
        string pointString = to_string(x) + "," + to_string(y) + "," + to_string(z);
        if (pointSet.find(pointString) == pointSet.end()) {
            points.push_back({x, y, z});
            pointSet.insert(pointString);
        }
    }
    return points;
}

//IMPRIME EL DIBUJO DEL ARBOL
void printKdTree(Node* node, string indent = "") {
    if (node == nullptr) {
        return;
    }

    stringstream pointStream;
    pointStream << "[";
    for(int i=0; i<node->point.size(); i++){
        pointStream << node->point[i];
        if(i != node->point.size()-1){
            pointStream << ", ";
        }  
    }
    pointStream << "]";

    cout << indent << pointStream.str() << endl;

    printKdTree(node->left, indent + "  ");
    printKdTree(node->right, indent + "  ");
}
//:.::::::::::::....::::LINK Y RANGE:::::::::::.::.....:::::::.
bool Link(vector<vector<int>> &points, vector<int> &point, int start, int end, int depth) {
    if (start > end) return false;
    int k = points[0].size();
    int axis = depth % k;
    int mid = (start + end) / 2;
    //La lina de abajo se ocupa para poder ver con qué punto está comparando el punto que estamos buscando
    //cout << "Comparando el punto (" << point[0] << "," << point[1] << ") con el punto (" << points[mid][0] << "," << points[mid][1] << ") en el eje " << (axis == 0 ? "X" : "Y") << endl;
    if (points[mid] == point) return true;
    if (point[axis] < points[mid][axis]) return Link(points, point, start, mid - 1, depth + 1);
     else if (point[axis] > points[mid][axis]) return Link(points, point, mid+1, end , depth + 1);
     else {
        // Si los puntos son iguales en el eje actual o si el punto es mayor, buscar primero en el subárbol izquierdo
        bool sonIguales = Link(points, point, start, mid - 1, depth + 1);
        if (sonIguales) {
            // Si se encontró el punto en el subárbol izquierdo, no es necesario buscar en el subárbol derecho
            return true;
        } else {
            // Si no se encontró el punto en el subárbol izquierdo, buscar en el subárbol derecho
            return Link(points, point, mid + 1, end, depth + 1);
        }
   }
}

int IntersectRectangles(Rectangle rect1, Rectangle rect2) {
    // Verificar si uno de los rectángulos está a la izquierda del otro
    if (rect1.top_right.x < rect2.bottom_left.x || rect2.top_right.x < rect1.bottom_left.x) {
        return 0;
    }
    // Verificar si uno de los rectángulos está por encima del otro
    if (rect1.top_right.y < rect2.bottom_left.y || rect2.top_right.y < rect1.bottom_left.y) {
        return 0;
    }
    // Si ninguna de las condiciones anteriores se cumple, los rectángulos se intersectan
    return 1;
}
vector<vector<int>> Range(Rectangle Q, Rectangle R, vector<vector<int>> pointsArray, int li, int ls, int x) {
    vector<vector<int>> pointsInRange;
    if (li <= ls ) {
        int k = floor((ls + li)/2);
        Point s = {pointsArray[k][0], pointsArray[k][1]};
        Rectangle r;
        r.bottom_left=s;
        r.top_right=s;

        Rectangle r1, r2;
        r1 = R ;
        r2 = R ;
        if(x==1) {
            r1.top_right.x=s.x;
            r2.bottom_left.x=s.x;
        }
        else {
            r1.top_right.y=s.y;
            r2.bottom_left.y=s.y;
        }
        if (IntersectRectangles(Q,R) == 1) {
            if (IntersectRectangles(r,Q)) {
                pointsInRange.push_back({s.x, s.y});
            }
            if(x==1) {
                vector<vector<int>> leftPoints = Range(Q,r1,pointsArray,li, k-1, 0);
                pointsInRange.insert(pointsInRange.end(), leftPoints.begin(), leftPoints.end());
                vector<vector<int>> rightPoints = Range(Q,r2,pointsArray,k+1, ls,0);
                pointsInRange.insert(pointsInRange.end(), rightPoints.begin(), rightPoints.end());
            }
            else {
                vector<vector<int>> rightPoints = Range(Q,r2,pointsArray,k+1, ls,1);
                pointsInRange.insert(pointsInRange.end(), rightPoints.begin(), rightPoints.end());
                vector<vector<int>> leftPoints = Range(Q,r1,pointsArray,li, k-1, 1);
                pointsInRange.insert(pointsInRange.end(), leftPoints.begin(), leftPoints.end());
            }
        }
    }
    return pointsInRange;

}
vector<int> Cuadrante1(int x,int y,int L,int minX,int minY,int maxX,int maxY){
    // Calcular la esquina inferior izquierda
    int inferiorIzquierdaX = max(minX,x-L);
    int inferiorIzquierdaY = max(minY,y-L);

    // Calcular la esquina superior derecha
    int superiorDerechaX = min(maxX,inferiorIzquierdaX+L);
    int superiorDerechaY = min(maxY,inferiorIzquierdaY+L);

    return {inferiorIzquierdaX,inferiorIzquierdaY,superiorDerechaX ,superiorDerechaY };
}

vector<int> Cuadrante2(int x,int y,int L,int minX,int minY,int maxX,int maxY){
    // Calcular la esquina inferior derecha
    int inferiorDerechaX = min(maxX,x+L);
    int inferiorDerechaY = max(minY,y-L);

    // Calcular la esquina superior izquierda
    int superiorIzquierdaX = max(minX,inferiorDerechaX-L);
    int superiorIzquierdaY = min(maxY,inferiorDerechaY+L);

    return {superiorIzquierdaX ,superiorIzquierdaY ,inferiorDerechaX ,inferiorDerechaY };
}

vector<int> Cuadrante3(int x,int y,int L,int minX,int minY,int maxX,int maxY){
   // Calcular la esquina superior derecha
   int superiorDerechaX = min(maxX,x+L);
   int superiorDerechaY = min(maxY,y+L);

   // Calcular la esquina inferior izquierda
   int inferiorIzquierdaX = max(minX ,superiorDerechaX-L);
   int inferiorIzquierdaY = max(minY ,superiorDerechaY-L);

   return {inferiorIzquierdaX ,inferiorIzquierdaY ,superiorDerechaX ,superiorDerechaY };
}

vector<int> Cuadrante4(int x,int y,int L,int minX,int minY,int maxX,int maxY){
   // Calcular la esquina superior izquierda
   int superiorIzquierdaX = max(minX,x-L);
   int superiorIzquierdaY = min(maxY,y+L);

   // Calcular la esquina inferior derecha
   int inferiorDerechaX = min(maxX,superiorIzquierdaX+L);
   int inferiorDerechaY = max(minY,superiorIzquierdaY-L);

   return {superiorIzquierdaX,superiorIzquierdaY,inferiorDerechaX,inferiorDerechaY};
}
vector<int> calcularCuadrado(int x, int y, int L, int minX, int minY, int maxX, int maxY) {
    int mitadL = L / 2;

    int inferiorIzquierdaX = x - mitadL;
    int inferiorIzquierdaY = y - mitadL;
    int superiorDerechaX = x + mitadL;
    int superiorDerechaY = y + mitadL;
    // Verificar si el cuadrado se sale de los límites de la matriz
    if (L % 2 == 0 && inferiorIzquierdaX >= minX && inferiorIzquierdaY >= minY && superiorDerechaX <= maxX && superiorDerechaY <= maxY) {
        return {inferiorIzquierdaX, inferiorIzquierdaY, superiorDerechaX, superiorDerechaY};
    } else {
        // ESsto se hace basicamente para ver por qué no se pudo generar el punto medio
        /*
        if (L % 2 != 0) {
            //cout << "No se puede usar el punto dado como punto medio porque L es impar." << endl;
        }
        if (inferiorIzquierdaX < minX || inferiorIzquierdaY < minY || superiorDerechaX > maxX || superiorDerechaY > maxY) {
            //cout << "No se puede usar el punto dado como punto medio porque el cuadrado generado se sale de los límites de la matriz" << endl;
        }*/
        // Verificar en qué cuadrante se encuentra el punto
        if (x <= (minX + maxX) / 2 && y <= (minY + maxY) / 2) {
            // Cuadrante 1
            return Cuadrante1(x, y, L, minX, minY, maxX, maxY);
        } else if (x > (minX + maxX) / 2 && y <= (minY + maxY) / 2) {
            // Cuadrante 2
            return Cuadrante2(x, y, L, minX, minY, maxX, maxY);
        } else if (x > (minX + maxX) / 2 && y > (minY + maxY) / 2) {
            // Cuadrante 3
            return Cuadrante3(x, y, L, minX, minY, maxX, maxY);
        } else {
            // Cuadrante 4
            return Cuadrante4(x,y,L,minX,minY,maxX,maxY);
        }
    }
}
void imprimirRectangulos(vector<Rectangle> rectangulos) {
    for (int i = 0; i < rectangulos.size(); i++) {
        Rectangle rect = rectangulos[i];
        cout << "Rectángulo " << i + 1 << ": (" << rect.bottom_left.x << ", " << rect.bottom_left.y << "), (" << rect.top_right.x << ", " << rect.top_right.y << ")" << endl;
    }
}
bool existeRectangulo(vector<Rectangle> rectangulos, Rectangle c) {
    for (const auto& r : rectangulos) {
        if (r.bottom_left.x == c.bottom_left.x && r.bottom_left.y == c.bottom_left.y && r.top_right.x == c.top_right.x && r.top_right.y == c.top_right.y) {
            return true;
        }
    }
    return false;
}
void escribirRectangulos(vector<Rectangle> rectangulos, string filename, vector<vector<int>> PointsArrays) {
    ofstream file(filename);

    for (const auto& rect : rectangulos) {
        file << rect.bottom_left.x << " " << rect.bottom_left.y << " " << rect.top_right.x << " " << rect.top_right.y << endl;
    }

    file << endl;

    for (const auto& point : PointsArrays) {
        file << point[0] << " " << point[1] << endl;
    }

    file.close();
}

void promedioRange(int L, vector<vector<int>> PointsArrays, int k, Rectangle R) {

    Rectangle c;
    int x,y;
    int minX = R.bottom_left.x;
    int minY= R.bottom_left.y;
    int maxX= R.top_right.x;
    int maxY= R.top_right.y;
    vector<Rectangle> rectangulos(k);
    vector<int> cuadrados(k);
    for(int i=0;i<k; i++){
        int randomIndex = rand() % PointsArrays.size();
        vector<int> randomPoint = PointsArrays[randomIndex];
        x = randomPoint[0];
        y = randomPoint[1];
        cuadrados = calcularCuadrado(x,y,L,minX,minY,maxX,maxY);
        c.bottom_left.x = cuadrados[0];
        c.bottom_left.y = cuadrados[1];
        c.top_right.x = cuadrados[2];
        c.top_right.y = cuadrados[3];
        //para que no se repitan rectangulos
            
            if (!existeRectangulo(rectangulos, c)) {
                rectangulos[i]= c;
                cuadrados.clear();
            } else {
                i--;
            }
    }
    //tambien para ver si se guardaron bien los rectangulos
    //imprimirRectangulos(rectangulos);
    //SET TIME INICIO. Aqui deberiamos empezar a medir el tiempo

    for(int i = 0; i < k; i++){

        vector<vector<int>> pointsInRange = Range(rectangulos[i],R,PointsArrays,0, PointsArrays.size() - 1, 1);
    
        cout << "RECTANGULO " << (i+1) << " (" << rectangulos[i].bottom_left.x << "," << rectangulos[i].bottom_left.y << ") (" << rectangulos[i].top_right.x << "," << rectangulos[i].top_right.y << ")" << endl;
    
        cout << "Puntos en el rango:" << endl;
        //Para mostrar los puntos que estan en el rectangulo
        for (const auto& point : pointsInRange) {
            cout << "(" << point[0] << "," << point[1] << ")" << endl;
        }
        
    }
    //SET TIME END. Aqui deberiamos terminar de medir el tiempo
    // Crear un vector de rectángulos para imprimirlos y posteriormente graficarlos con gnuplot
    vector<Rectangle> rectan = rectangulos;
        
    escribirRectangulos(rectan, "rectangulos.txt",PointsArrays);

}




int main() {
    int npoints = 429000000;
    int maxcoordinate=2048;
    vector<vector<int>> puntosRandoms = generateRandomPoints3D(npoints, maxcoordinate);

// Ejemplo de como usar ckd-tree para generar los codigos en espiral 
    //int npoints = 16;
    //int maxcoordinate= 20;
    //vector<vector<int>> puntosRandoms= {{7,3}, {2,4}, {7,6}, {3,1}, {2,7}, {7,4}, {8,9}, {1,3}, {4,2},{1,9},{3,7},{6,5},{8,1},{6,8},{9,9},{0,1}};

    KdTree tree(puntosRandoms);

    tree.generarCodigos();
    //tree.printCodes(tree.getRoot());
    vector<uint> codes(puntosRandoms.size());

    codigosToArray(tree.getRoot(),codes,0,puntosRandoms.size()-1);
   // cout << "IMPRESION DE LOS CODIGOS EN EL ARREGLO"<< endl;
    //imprimirCodigosDeArreglo(codes);

//--------- Se imprimen los codigos en espiral----- 
   vector<int> q= tree.getRootPoint();
   printf("%d %d %d %d %d \n", npoints, maxcoordinate, q[0], q[1], q[2]);
   for (int l= 0 ; l < npoints; l++) 
        printf("%d\n", codes[l]);

    return 0;
}
