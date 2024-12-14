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
 int Range(struct Point parent, FTRep *kd, Rectangle Q, Rectangle R, int li, int ls, int x) {
   int tp=0;
   if (li <= ls ) {
      int k = floor((ls + li)/2);
      struct Point s;
      decode(accessFT(kd, k),parent, &s);
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
        //if (IntersectRectangles(r,Q)) printf("(%d, %d) \n", s.x,s.y);
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



int main(int argc, char* argv[]){
    uint * list, bytesFile, listLength,i, ox,oy;
    int maxCoordinate;
    // Primera linea
    // numero de puntos, coordenada x  y coordenada y del punto del medio del KD-tree
    scanf("%d %d %d %d" , &listLength, &maxCoordinate, &ox,&oy);
    //int s = (int) floor(sqrt(code));
    list = (uint *) malloc(sizeof(uint)*listLength);
    // un código por linea
    for (int l = 0; l < listLength; l++) scanf("%d",(list +l));

    //------------------------------------------------------
    //se crea el DACS
    //------------------------------------------------------
    FTRep* listRep = createFT(list,listLength);
	//printf("Tamaño %d\n",memoryUsage(listRep));

    int code = accessFT(listRep, 6);
    struct Point q; 
    struct Point p; 
    p.x =ox, p.y=oy; 
    decode(code, p, &q); 
    printf("Codigo DACS: %d Punto (%d, %d)\n", code, q.x, q.y); 
  //-------------------------------------------------
	/*Test: decompression*/
/*
	listaux = decompressFT(listRep,listLength);
	uint conterrors = 0;
   	for(i=0;i<listLength;i++)
		if(listaux[i]!=list[i]){
			fprintf(stderr,"Error on decompressFT, the %d-th decompressed element does not match with the original list\n",i+1);
			conterrors ++;
		}
	fprintf(stderr," Test: decompression -> %d mismatches\n", conterrors);
	free(listaux);
*/
	/*Test: direct access*/
/*
	uint value;
	conterrors = 0;
	for(i=0;i<listLength;i++){
		value = accessFT(listRep, i+1);
		printf("i = %d %d %d \n", i, list[i], value);
		if(value!=list[i]){
			conterrors ++;
			fprintf(stderr,"Error on decompressFT, the %d-th decompressed element does not match with the original list\n",i+1);
		}
	}
	fprintf(stderr," Test: direct access -> %d mismatches\n", conterrors);
*/
	saveFT(listRep,"out");
    printf(stderr,"\n\n Representation saved.\n");

    destroyFT(listRep);
	free(list);
} 








