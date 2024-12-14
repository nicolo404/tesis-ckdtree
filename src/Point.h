class Point { 
 public:
  int x;
  int y;
  Point(int x, int y) { 
  this->x=x;
  this->y=y;
}
 void print(){printf("(%d,%d)", x,y);}
 
 int Equals(Point p) { 
  if (p.x ==this->x && p.y == this->y) return 1; 
  else return 0;  
 } 
};
