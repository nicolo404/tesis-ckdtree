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


//Esta función determina si dos rectángulos se intersectan o no
// Me le construyó chatGPT :-) y está bien ..le daremos las gracias
//
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

 // ------------------------------------------------------------------------------------------
 void Range(struct Point parent, FTRep *kd, Rectangle Q, Rectangle R, int li, int ls, int x) {
// parent y kd no debería ir, en el caso de kd debería ir el arreglo con los puntos del KD-tree
// Q es el retángulo de la consulta y R el rectangulo con la region/subreggión donde continuar
   // la bśuqeda. lis y ls indican los lí,ites del arreglo donde continuar la búsqueda y x indica
   // si la particion es vertical u horaizonal (x=0 en dirección del eje x y x=1 en dirección del eje y)

   if (li <= ls ) {
      int k = floor((ls + li)/2); //El DACs comienza en 1 y el arreglo del KD_tree en 0...ojo
      struct Point s;
      decode(accessFT(kd, k),parent, &s); // esta linea no la necesitarias. Debes accesar directamente el punto con k
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
         if (IntersectRectangles(r,Q)) printf("(%d, %d) \n", s.x,s.y); // el punto esta dentro de Q
         // if (IntersectRectangles(r,Q))s=s; // no se agrega a ninguna lista
        if(x==1) {
          Range(s, kd, Q,r1,li, k-1, 0);
          Range(s, kd, Q,r2, k+1, ls,0);
        }
       else {
         Range(s, kd, Q,r2,k+1, ls,1);
         Range(s, kd, Q,r1,li, k-1, 1);
       }
     }
   }
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
  // parent es el punto que se usa para decodificar, q es el punto que se está buscando
  // kd es el DACs y que debes reemplazar por el ARREGLO donde se almacena el KD-tree
  // li, ls indican los límites donde buscar en el arreglo, y x es una variable para
  // indicar si la partición es vertical u horizontal (x=0, división por la x y x=1, división
  // por eje y
  //
  //-----------------------------------------------------------------------------------------
     if (ls < li) return 0; // No esta el punto q
     int i = floor((ls + li)/2); //En el caso del DACs, la secuencia parte de 1; en tu caso de 0.
     struct Point p;
     decode(accessFT(kd, i),parent, &p); //Esta línea no iria. Debes accesar directamente el punto usando el índice i
     if (p.x== q.x && p.y == q.y) return 1; //se encontró el punto.
     if(x==1) {
       if (q.x <= p.x) return Link(p,q, kd, li,i-1,0);
         else return Link(p,q, kd, i+1,ls,0);
       }
      else{
        if (q.y <= p.y) return Link(p,q, kd, li,i-1,1);
          else return Link(p,q, kd, i+1,ls,1);
       }
  }

void decode(int code,  struct Point parent, struct Point *q){
    if(code == 0){
        q->x =parent.x;
        q->y =parent.y;
    }
    else {
    int sqr = (int) floor(sqrt(code));
    int length = (sqr % 2) + 1 + sqr;
    int max = (length - 1) >> 1;
    int initialPosition = (length-2) * (length-2);
    int diff = code - initialPosition;
    int zone = diff / (length-1);
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



int main(int argc, char* argv[])
{


    uint * list, bytesFile, listLength,i, ox,oy;
    int maxCoordinate;
       // Primera linea 
       // numero de puntos, coordenada x  y coordenada y del punto del medio del KD-tree
        scanf("%d %d %d %d" , &listLength, &maxCoordinate, &ox,&oy);
       //int s = (int) floor(sqrt(code));
	list = (uint *) malloc(sizeof(uint)*listLength);
        
        // un código por linea 
        for (int l = 0; l < listLength; l++) scanf("%d",(list +l));
//        for (int l = 0; l < listLength; l++) printf("%d\n",list[l] );

	// arreglo con numeros al azar entre 0 y 999
	//for (int i=0; i < listLength; i++)
	//	 list[i]=rand()%1000;

	// Se crea el DACs
	FTRep* listRep = createFT(list,listLength);
    struct Point q;
    struct Point p;
    struct timeval ti, tf;// Para medir el tiempo.
    double tiempo;
    p.x =ox, p.y=oy;
    q.x = 0;
    q.y =1;

  // Se preparan 1000 queries para Link.
    int nq=1000;
    struct Point *queries = (struct Point *) malloc(sizeof(struct Point)*nq);
    int l=0;
   while(l < nq) {
      int k= rand() % listLength-1;
      if(k !=0) {
       Access(p, k, (queries + l), listRep, 1, listLength);
       l++;
      }
    }

    // Se mide el tiempo (milisegundos)  de ejecución 2*nq queries (Link()).
    gettimeofday(&ti, NULL);
    for (int i=0; i <nq;i++) {
        Link(p, *(queries +i), listRep, 1, listLength, 1); //query exitoso
        (queries+i)->x = (queries+i)->x + maxCoordinate; // Se ajustan las coordenadas
        (queries+i)->y = (queries+i)->y + maxCoordinate; // para realizar una busqueda infructuosa
        Link(p, *(queries +i), listRep, 1, listLength, 1); //Query infructuoso
    }
    gettimeofday(&tf, NULL);
    tiempo= ((tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0)/(2*nq);
    printf( " Tiempo promedio Link() (ms) %f\n", tiempo);
/*
    struct Point s;
    printf("<---> %d\n ", Access(p, 8, &s, listRep, 1, listLength));
    printf("(%d,%d)\n", s.x,s.y);
    Interval(p, listRep, 1, listLength, 1, 16);

    Rectangle R, Q;
    R.bottom_left.x = 0;
    R.bottom_left.y = 0;
    R.top_right.x=maxCoordinate;
    R.top_right.y=maxCoordinate;

    Q.bottom_left.x = 1;
    Q.bottom_left.y = 1;
    Q.top_right.x= 4;
    Q.top_right.y= 5;

    printf("-------------\n");



    gettimeofday(&ti, NULL);
//   Range(p, listRep,  Q, R,  1, listLength, 1);
    gettimeofday(&tf, NULL);
    tiempo= (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
    printf( "Tiempo Milisegundos %f\n", tiempo);
    uint * listaux;
    */

    /*-----------------------------------------
    int code = accessFT(listRep, 6);
    struct Point q; 
    struct Point p; 
    p.x =ox, p.y=oy; 
    decode(code, p, &q); 
    printf("%d -- %d %d -- %d %d\n", code, q.x, q.y, p.x, p.y); 
 */
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








