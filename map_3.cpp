#include <GL/glut.h>
#include <iostream>
#include <stdio.h>
//#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#define WIDTH 600
#define HEIGHT 400
#define MOVE 1
#define QUIET 2

void display(); //int col, int row
void keyboard(unsigned char c,int x,int y);
void flechas(int key, int x, int y);
void recursion(int r, int c);
int * generateDirection();
void print();
void idle();

int keyflag=0;
int i, j;
int ROWS;
int COLUMNS;
char **tablero;
int ia, ja;
float ancho, alto; 
int x_, y_; // Position in the matrix
class particle {
  float x,y;   //-- Current position
  float vx,vy; //-- Velocity vector
  int state;
  
  long time_remaining;

public:

  particle();
  void set_position(int x,int y);
  void init_movement(int destination_x,int destination_y,int duration);
  void integrate(long t);
  void draw(); 
};


//-----------------------------------------------

particle::particle()
{
  state=QUIET;
}

//-----------------------------------------------

void particle::set_position(int x,int y)
{

  x_ = x;
  y_ = y;

	/*if (x == 1 && y ==1)
	{
		this->x = x*(ancho +(WIDTH/COLUMNS)/2);
	  	this->y = y*(alto+ (HEIGHT/ROWS)/2);
	}else
	{*/
		this->x = x*ancho;
  		this->y = y*alto;
	//}
  
}

//-----------------------------------------------

void particle::init_movement(int destination_x,int destination_y,int duration)
{

	printf("%f LLega\n", destination_x * ancho);
	//printf("%f x* ancho \n", x * ancho);
	printf("%d des_y %d des_x %f ancho %f alto %f y %f x \n", destination_y, destination_x, ancho, alto, y, x);
	printf("%f des*alto\n", destination_y *alto);
	//printf("%f y*alto\n", y *alto);

  x_ = destination_x;
  y_ = destination_y;

  /*if(x!=x_)
  {
  	vx = (((destination_x*ancho) - x )-(WIDTH/COLUMNS)/2)/duration;
  	vy = (destination_y*alto - y)/duration;
  }else if(y!=y_)
  {*/
  	vx = (destination_x*ancho - x )/duration;
 	vy = (((destination_y*alto) - y)/duration);//-(HEIGHT/ROWS)/2);
  //}
  

  state=MOVE;
  time_remaining=duration;
}

//-----------------------------------------------

void particle::integrate(long t)
{

  if(state==MOVE && t<time_remaining)
    {
      x = x + vx*t -(WIDTH/COLUMNS)/2;
      y = y + vy*t -(HEIGHT/ROWS)/2;
      time_remaining-=t;
    }
  else if(state==MOVE && t>=time_remaining)
    {
      x = x + vx*time_remaining -(WIDTH/COLUMNS)/2;
      y = y + vy*time_remaining  -(HEIGHT/ROWS)/2;
      state=QUIET;
    }
    //printf("%f x, %f y\n", x,y);
}

//-----------------------------------------------

void particle::draw()
{
  glColor3f(0.4,0.8,0.6);
  glBegin(GL_QUADS);
  /*glVertex2i(((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2)-6,((HEIGHT/ROWS)+(HEIGHT/ROWS)/2)-6);
  glVertex2i(((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2)+6,((HEIGHT/ROWS)+(HEIGHT/ROWS)/2)-6);
  glVertex2i(((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2)+6,((HEIGHT/ROWS)+(HEIGHT/ROWS)/2)+6);
  glVertex2i(((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2)-6,((HEIGHT/ROWS)+(HEIGHT/ROWS)/2)+6);
  */glVertex2i(x-6,y-6);
  glVertex2i(x+6,y-6);
  glVertex2i(x+6,y+6);
  glVertex2i(x-6,y+6);
  glEnd();
}



particle bueno;
particle malo;
long last_t=0;


int main(int argc,char *argv[]) {

	if(argc == 1)
	{
		printf("Insertar alto y ancho del tablero.\n");
		exit(0);
	}

	ROWS = atoi(argv[1]);
	COLUMNS = atoi(argv[2]);	

	ancho=(WIDTH/COLUMNS +(WIDTH/COLUMNS)/2);
	alto = (HEIGHT/ROWS + (HEIGHT/ROWS)/2);


	int k;
	tablero = (char **) malloc((ROWS)*sizeof(char*));
	for(k=0; k<ROWS;k++)
	{
		tablero[k]=(char*) malloc(COLUMNS*sizeof(char));
	}

	for (ja=0;ja<ROWS;ja++)
	{
		for (ia=0;ia<COLUMNS;ia++)
		{
			tablero[ja][ia]='1';
		}
	}

	srand(time(NULL));
	
	int r = 1;
	int c = 1;

	tablero[r][c]='0';
	tablero[r][COLUMNS-c-1]='0';

	recursion(r, c);
	print();

	bueno.set_position(1,1);
	/*bueno.set_position(((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2),(HEIGHT/ROWS)+(HEIGHT/ROWS)/2);
	malo.set_position(WIDTH - ((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2),(HEIGHT/ROWS)+(HEIGHT/ROWS)/2);
*/
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50,50);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Map");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(flechas);
  	glutIdleFunc(idle);

	glMatrixMode(GL_PROJECTION);
	glScalef(1.0f, -1.0f, 1.0f);
	gluOrtho2D(0, WIDTH-1,0,HEIGHT-1);

	glutMainLoop();

    return 0;

}


void recursion(int r, int c)
{
	int * ranDir;
	ranDir = generateDirection();
	int i;
	for (i=0; i<4;i++)
	{
		switch(ranDir[i]) {

	   case 1  :
	      if(r-2 <= 0 || (r-2 == 1 && c == 1))
	      {
	      	continue;
	      }
	      if(tablero[r-2][c]!='o')
	      {
	      		tablero[r-2][c]='o';
	      		tablero[r-1][c]='o';
	      		tablero[r-2][COLUMNS -c -1]='o';
	      		tablero[r-1][COLUMNS -c -1]='o';
	      		recursion(r-2,c);
	      }
	      break; 
		
	   case 2  :
	      if(c+2 >= ((COLUMNS/2)+1))
	      {
	      	continue;
	      }
	      if(tablero[r][c+2]!='o')
	      {
	      		tablero[r][c+2]='o';
	      		tablero[r][c+1]='o';
	      		tablero[r][COLUMNS - (c+2) - 1]='o';
	      		tablero[r][COLUMNS - (c+1) - 1]='o';
	      		recursion(r,c+2);
	      }
	      break;

	    case 3  :
	      if(r+2 >= ROWS - 1)
	      {
	      	continue;
	      }
	      if(tablero[r+2][c]!='o')
	      {
	      		tablero[r+2][c]='o';
	      		tablero[r+1][c]='o';
	      		tablero[r+2][COLUMNS -c -1]='o';
	      		tablero[r+1][COLUMNS -c -1]='o';
	      		recursion(r+2,c);
	      }
	      break; 
		
	   case 4  :
	      if(c-2 <= 0 || (r == 1 && c-2 == 1))
	      {
	      	continue;
	      }
	      if(tablero[r][c-2]!='o')
	      {
	      		tablero[r][c-2]='o';
	      		tablero[r][c-1]='o';
	      		tablero[r][COLUMNS - (c-2) -1]='o';
	      		tablero[r][COLUMNS - (c-1) -1]='o';
	      		recursion(r,c-2);
	      }
	      break; 
}
	}
}

int* generateDirection()
{
	static int Dir[4];
	int i;
	for (i = 0; i<4;i++)
	{
		int raux = (rand() % 4) +1;
		if(Dir[0]!=raux && Dir[1]!=raux && Dir[2]!=raux)
		{
			Dir[i]=raux;
		}
		else
		{
			i--;
		}
	}		
	return Dir;

}

void print()
{
 	for (ja=0;ja<ROWS;ja++) //col
	{
		for (ia=0;ia<COLUMNS;ia++) //row
		{
			printf("%c ", tablero[ja][ia]);
		}
		printf("\n");
	}
}	


void display() 
{
	
 	glClearColor(1.0,1.0,1.0,1.0);
 	glClear(GL_COLOR_BUFFER_BIT);
 	bueno.draw();
 	malo.draw();

	for (j=0;j<ROWS;j++) 
	{
		for (i=0;i<(COLUMNS);i++) 
		{
			if ((keyflag==0 || keyflag==1) && tablero[j][i] == '1')
			{
				glColor3f(0.0,0.0,0.0);
				glBegin(GL_QUADS);

				glVertex2i(i*WIDTH/COLUMNS,j*HEIGHT/ROWS); 
				glVertex2i((i+1)*WIDTH/COLUMNS,j*HEIGHT/ROWS); 
				glVertex2i((i+1)*WIDTH/COLUMNS,(j+1)*HEIGHT/ROWS); 
				glVertex2i(i*WIDTH/COLUMNS,(j+1)*HEIGHT/ROWS); 

				glEnd();
							
			}
			else if((keyflag==0 || keyflag==1) && tablero[j][i] == 'o')
			{
				glColor3f(0.6,0.4,0.8);
				glBegin(GL_QUADS);

				glVertex2i(i*WIDTH/COLUMNS + (WIDTH/COLUMNS)/3,j*HEIGHT/ROWS + (HEIGHT/ROWS)/3); 
				glVertex2i((i+1)*WIDTH/COLUMNS - (WIDTH/COLUMNS)/3,j*HEIGHT/ROWS + (HEIGHT/ROWS)/3); 
				glVertex2i((i+1)*WIDTH/COLUMNS  - (WIDTH/COLUMNS)/3,(j+1)*HEIGHT/ROWS -((HEIGHT/ROWS)/3)); 
				glVertex2i(i*WIDTH/COLUMNS + (WIDTH/COLUMNS)/3, (j+1)*HEIGHT/ROWS - ((HEIGHT/ROWS)/3)); 

				glEnd();
			}else if((keyflag==0 || keyflag==1) && tablero[j][i] == 'P')
			{
				glColor3f(0.4,0.8,0.6);
				glBegin(GL_QUADS);

				glVertex2i(i*WIDTH/COLUMNS + (WIDTH/COLUMNS)/4,j*HEIGHT/ROWS + (HEIGHT/ROWS)/4); 
				glVertex2i((i+1)*WIDTH/COLUMNS - (WIDTH/COLUMNS)/4,j*HEIGHT/ROWS + (HEIGHT/ROWS)/4); 
				glVertex2i((i+1)*WIDTH/COLUMNS  - (WIDTH/COLUMNS)/4,(j+1)*HEIGHT/ROWS -((HEIGHT/ROWS)/4)); 
				glVertex2i(i*WIDTH/COLUMNS + (WIDTH/COLUMNS)/4, (j+1)*HEIGHT/ROWS - ((HEIGHT/ROWS)/4)); 

				glEnd();
			}
		}
		
	}
	glutSwapBuffers();
}


//-----------------------------------------------
//-----------------------------------------------
void keyboard(unsigned char c,int x,int y)
{
  /*if(keyflag==0)
    keyflag=1;
  else
    keyflag=0;*/
  	//bueno.set_position(50,50);
	//bueno.init_movement(WIDTH-((WIDTH/COLUMNS)+(WIDTH/COLUMNS)/2),(HEIGHT/ROWS)+(HEIGHT/ROWS)/2,1000);
	bueno.init_movement(1,5,1000);
	//bueno.set_position(1,5);

  glutPostRedisplay();

};

void flechas(int key, int x, int y)
{
	printf("%d\n", key);
	switch (key) 
	{
		case 100:
		{ 
			if(tablero[y_][x_-1] != '1')
			{
				bueno.init_movement(x_-1, y_,1000); //left
			}
		}
		break; 	
		case 101: 
		{
			if(tablero[y_-1][x_] != '1')
				{			
					bueno.init_movement(x_, y_-1,1000);//up
				}
		}
		break; 
		case 102: 
		{
			if(tablero[y_][x_+1] != '1')
			{
				bueno.init_movement(x_+1, y_,1000); //Right		
			}
		}
		break; 
		case 103: 
		{
			if(tablero[y_+1][x_] != '1')
			{
				bueno.init_movement(x_, y_+1,1000); break; //Down
			}
		}
		break;

	}
	glutPostRedisplay();
}

void idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME); 

  if(last_t==0)
    last_t=t;
  else
    {
      bueno.integrate(t-last_t);
      last_t=t;
    }


  glutPostRedisplay();
}
