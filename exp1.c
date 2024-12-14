#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include "constants.h"
#include "dacs.h"
#include <sys/time.h>

struct Point {
 int x; 
 int y;
 int z; 
 };

 typedef struct {
   struct  Point bottom_left;
   struct  Point top_right;
} Rectangle;



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
//
//------Rectangulo dentro de un rectángulo 
 
int InsideRectangle(Rectangle rect1, Rectangle rect2) {
    if(((rect1.bottom_left.x >= rect2.bottom_left.x) && 
     (rect1.bottom_left.y >= rect2.bottom_left.y)) &&
     ((rect1.top_right.x <= rect2.top_right.x) &&
     (rect1.top_right.y <= rect2.top_right.y))) return 1; 
    else  return 0;
   }



//---- Rango sobre la estructura compactada (DACS)------
 int Range(struct Point parent, FTRep *kd, Rectangle Q, Rectangle R, int li, int ls, int x) {
   int tp=0;
//	printf("R: %d %d %d %d \n", R.bottom_left.x, R.bottom_left.y, R.top_right.x, R.top_right.y);
//	printf("Q: %d %d %d %d \n", Q.bottom_left.x, Q.bottom_left.y, Q.top_right.x, Q.top_right.y);
   if (li <= ls ) {
      int k = floor((ls + li)/2);
      struct Point s;
      decode(accessFT(kd, k),parent, &s);
 //     printf("Mediana %d %d \n",s.x, s.y);
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
	//printf("r: %d %d %d %d \n", r.bottom_left.x, r.bottom_left.y, r.top_right.x, r.top_right.y);
	//printf("r1: %d %d %d %d \n", r1.bottom_left.x, r1.bottom_left.y, r1.top_right.x, r1.top_right.y);
	//printf("r2: %d %d %d %d \n", r2.bottom_left.x, r2.bottom_left.y, r2.top_right.x, r2.top_right.y);

	//if (InsideRectangle(R,Q)) tp = tp + (ls-li) +1;
        //else 
        if (IntersectRectangles(Q,R) == 1) {
        //if (IntersectRectangles(r,Q) == 1) printf("(%d, %d) \n", s.x,s.y);
          if (IntersectRectangles(r,Q))tp++; // no se agrega a ninguna lista
        if(x==1) {
          if (IntersectRectangles(Q,r1))tp = tp+ Range(s, kd, Q,r1,li, k-1, 0);
          if (IntersectRectangles(Q,r2))tp = tp+ Range(s, kd, Q,r2, k+1, ls,0);
        }
       else {
         if (IntersectRectangles(Q,r2)) tp = tp + Range(s, kd, Q,r2,k+1, ls,1);
         if (IntersectRectangles(Q,r1)) tp = tp + Range(s, kd, Q,r1,li, k-1, 1);
       }
     }
   }
   return tp;
 }

 

// ---- Range directamente sobre el KD-tree, sin comprimir----
 int Range1(struct Point *kd, Rectangle Q, Rectangle R, int li, int ls, int x) {
   int tp=0;
//	printf("R: %d %d %d %d \n", R.bottom_left.x, R.bottom_left.y, R.top_right.x, R.top_right.y);
//	printf("Q: %d %d %d %d \n", Q.bottom_left.x, Q.bottom_left.y, Q.top_right.x, Q.top_right.y);
   if (li <= ls ) {
      int k = floor((ls + li)/2);
      struct Point s ; 
      s = kd[k]; 
   // decode(accessFT(kd, k),parent, &s);
 //     printf("Mediana %d %d \n",s.x, s.y);
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
	//printf("r: %d %d %d %d \n", r.bottom_left.x, r.bottom_left.y, r.top_right.x, r.top_right.y);
	//printf("r1: %d %d %d %d \n", r1.bottom_left.x, r1.bottom_left.y, r1.top_right.x, r1.top_right.y);
	//printf("r2: %d %d %d %d \n", r2.bottom_left.x, r2.bottom_left.y, r2.top_right.x, r2.top_right.y);

	//if (InsideRectangle(R,Q)) tp = tp + (ls-li) +1;
        //else 
        if (IntersectRectangles(Q,R) == 1) {
        //if (IntersectRectangles(r,Q) == 1) printf("(%d, %d) \n", s.x,s.y);
          if (IntersectRectangles(r,Q))tp++; // no se agrega a ninguna lista
        if(x==1) {
          if (IntersectRectangles(Q,r1))tp = tp+ Range1(kd, Q,r1,li, k-1, 0);
          if (IntersectRectangles(Q,r2))tp = tp+ Range1(kd, Q,r2,k+1,ls,  0);
        }
       else {
         if (IntersectRectangles(Q,r2)) tp = tp + Range1(kd, Q,r2,k+1, ls,1);
         if (IntersectRectangles(Q,r1)) tp = tp + Range1(kd, Q,r1,li, k-1, 1);
       }
     }
   }
   return tp;
 }


//-------------------------------
//

 void Interval(struct Point parent, FTRep *kd, int li, int ls, int i, int j) {
     if (li <= ls ) {
      int k = floor((ls + li)/2);
      struct Point s;
      decode(accessFT(kd, k),parent, &s);
      if (k >= i && k <=j) {
        Interval(s, kd, li, k-1, i,j);
        printf("(%d, %d) \n", s.x,s.y);
        Interval(s, kd, k+1, ls, i,j);
      }
      else {
      if(k > j)  Interval(s, kd, li, k-1, i,j);
      else Interval(s, kd, k+1, ls, i,j);
     }
   }
 }

 int Access(struct Point parent, int j, struct Point *s,  FTRep* kd, int li, int ls) {
 //    printf("li = %d ls = %d\n",li, ls);
     if (j< li || j >ls) return 0; // No esta
     int i = floor((ls + li)/2);
   //  printf("i %d %d\n",i, accessFT(kd,i));
     decode(accessFT(kd, i),parent, s);
    // printf("Parent : %d  %d\n", s->x, s->y);
     parent.x = s->x;
     parent.y = s->y;
     if (i==j) return 1;
     if (j < i) return Access(parent,j,s, kd, li,i-1);
      else  return Access(parent,j,s, kd, i+1,ls);
 }

int Link(struct Point parent, struct Point q, FTRep* kd, int li, int ls, int x) {
    // printf("li = %d ls = %d\n",li, ls);
     if (ls < li) return 0; // No esta
     int i = floor((ls + li)/2);
    // printf("i %d %d\n",i, accessFT(kd,i));
     struct Point p;
     decode(accessFT(kd, i),parent, &p);
    // printf("Parent : %d  %d\n", p.x, p.y);
     if (p.x== q.x && p.y == q.y) return 1;
     if(x==1) {
       if (q.x < p.x) return Link(p,q, kd, li,i-1,0);
         else if (q.x > p.x)  return Link(p,q, kd, i+1,ls,0);
       else { 
	       int r =  Link(p,q,kd,li,i-1,0);
	       if( r == 0) return Link(p,q,kd,i+1,ls,0);
	       else return r;
       } 
       }
      else{
        if (q.y < p.y) return Link(p,q, kd, li,i-1,1);
          else if (q.y > p.y) return Link(p,q, kd, i+1,ls,1);
	else { 
	       int r =  Link(p,q,kd,li,i-1,1);
	       if( r == 0) return Link(p,q,kd,i+1,ls,1);
	       else return r;
	}
   }
  }

//----Link1 

int Link1(struct Point q, struct Point * kd, int li, int ls, int x) {
     if (ls < li) return 0; // No esta
     int i = floor((ls + li)/2);
    // printf("i %d %d\n",i, accessFT(kd,i));
     struct Point p=kd[i];
     //decode(accessFT(kd, i),parent, &p);
    // printf("Parent : %d  %d\n", p.x, p.y);
     if (p.x== q.x && p.y == q.y) return 1;
     if(x==1) {
       if (q.x < p.x) return Link1(q, kd, li,i-1,0);
         else if (q.x > p.x)  return Link1(q, kd, i+1,ls,0);
       else { 
	       int r =  Link1(q,kd,li,i-1,0);
	       if( r == 0) return Link1(q,kd,i+1,ls,0);
	       else return r;
       } 
       }
      else{
        if (q.y < p.y) return Link1(q, kd, li,i-1,1);
          else if (q.y > p.y) return Link1(q, kd, i+1,ls,1);
	else { 
	       int r =  Link1(q,kd,li,i-1,1);
	       if( r == 0) return Link1(q,kd,i+1,ls,1);
	       else return r;
	}
   }
  }




void decode(uint code,  struct Point parent, struct Point *q){
    if(code == 0){
        q->x =parent.x;
        q->y =parent.y;
    }
    else {
    uint sqr = (uint) floor(sqrt(code));
    uint length = (sqr % 2) + 1 + sqr;
    uint max = (length - 1) >> 1;
    uint initialPosition = (length-2) * (length-2);
    uint diff = code - initialPosition;
    uint zone = diff / (length-1);
    //printf("Zone %d \n", zone);
    if(zone == 0){
         q->x = parent.x + max ;
         q->y = parent.y + max - diff;
     }else if(zone == 1){ 
        q->x= parent.x + (length -1)+ max - diff;
        q->y = parent.y - max;
     }else if(zone == 2){ 
         q->x=  parent.x - max ;
         q->y = parent.y + diff - max - 2*(length -1);
     }else {
        q->x= parent.x + diff - max - 3*(length -1) ;
        q->y= parent.y + max;
     }
    }
    //return p;
   
}
 void DecodePoints(uint *codes, int li, int ls, int ox, int oy, int oz, struct Point * P) { 
	 if (li <= ls ) { 
	    int m= (li +ls)/2;
	    struct Point parent,q; 
	    parent.x = ox; 
	    parent.y = oy;
      parent.z = oz; 
	    decode(codes[m],parent,(P+m));
	    //printf("%d %d - %d \n",P[m].x,P[m].y, codes[m]);
            DecodePoints(codes, li,m-1, P[m].x, P[m].y,P, P[m].z);
            DecodePoints(codes, m+1, ls, P[m].x, P[m].y,P[m].z, P);
	 }
 }
             
 

int main(int argc, char* argv[]){
    uint * list, bytesFile, listLength,i, ox,oy,oz;
    int maxCoordinate;
    // Primera linea
    // numero de puntos, coordenada x  y coordenada y del punto del medio del KD-tree
    scanf("%d %d %d %d %d" , &listLength, &maxCoordinate, &ox,&oy,&oz);
    
    //int s = (int) floor(sqrt(code));
    list = (uint *) malloc(sizeof(uint)*listLength);
    // un código por linea
    for (int l = 0; l < listLength; l++) scanf("%d",(list +l));

    struct Point * PointsArray = (struct Point *)  malloc(sizeof(struct Point)*listLength);
    DecodePoints(list, 0, listLength-1,ox,oy,oz, PointsArray);
    for(int h = 0; h < listLength; h++) printf("(%d,%d,%d)\n",PointsArray[h].x, PointsArray[h].y, PointsArray[h].z);
    //------------------------------------------------------
    //se crea el DACS
    //------------------------------------------------------
    printf("Creando DACS\n");
    FTRep* listRep = createFT(list,listLength);
    int code = accessFT(listRep, 7);
    printf("Codigo DACS: %d\n", code);
    saveFT(listRep,"out");
    printf(stderr,"\n\n Representation saved.\n");
    destroyFT(listRep);
    free(list);
} 
