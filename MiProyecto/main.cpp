#include<iostream>
#include<windows.h>
#include<conio2.h>
#include<cstdlib>
#include<ctime>
using namespace std;

const int ANCHO_PANTALLA = 80;
const int ALTO_PANTALLA = 25;
const int MAX_BALAS_JUGADOR = 5;
const int MAX_BALAS_ENEMIGAS = 10;
const int MAX_ENEMIGOS = 40;

class ObjetoJuego {
protected:
	int x, y;
	char simbolo;
	int color;
	bool activo;
	
public:
	ObjetoJuego() : x(0), y(0), simbolo(' '), color(WHITE), activo(false) {}
	ObjetoJuego(int px, int py, char s, int c): x(px), y(py), simbolo(s), color(c), activo(true){}
	
	virtual void dibujar() {
		if (activo){
			textcolor(color);
			gotoxy(x, y);
			cprintf("%c", simbolo);
		};
	}
	
	virtual void borrar(){
		gotoxy(x, y);
		cprintf(" ");
	}
	
	int getX(){return x;}
	int getY(){return y;}
	bool estaActivo() {return activo;}
	void desactivar(){activo = false;}
	void setPos(int nx, int ny) {x = nx; y = ny;}
	char getSimbolo(){return simbolo;}
	int getColor() {return color;}
	
	virtual void mover() = 0;
};

class Bala: public ObjetoJuego{
private:
	int velocidad;
	
public:
	Bala():ObjetoJuego(), velocidad(0){}
	Bala(int px, int py, int vel):
	ObjetoJuego(px, py, '|', WHITE), velocidad(vel){}
	
	void mover() override{
		if (!activo) return;
		borrar();
		y+= velocidad;
		
		if(y<2||y> ALTO_PANTALLA - 2){
			activo = false;
			return;
		}
		dibujar();
	}
};


int main (int argc, char *argv[]) {
	
	
	
	
	
	
	
	
	
	
	return 0;
}

