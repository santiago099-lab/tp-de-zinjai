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

class Enemigo: public ObjetoJuego{
protected:
	int vida;
	int puntos;
	
public:
	Enemigo(): ObjetoJuego(), vida(0), puntos(0){}
	Enemigo(int px, int py, char s, int c, int v, int p):ObjetoJuego(px, py, s, c), vida(v), puntos(p){}
	
	void mover() override{}
	
	void recibirDanio(){
		vida--;
		if(vida <= 0){
			activo = false;
		};
	}
	
	int getPuntos() {return puntos;}
	int getVida() {return vida;}
	
	void moverPosicion(int nx, int ny){
		borrar();
		x = nx;
		y = ny;
		dibujar();
	}
};

class EnemigoDebil: public Enemigo{
public:
	EnemigoDebil(): Enemigo(){}
	EnemigoDebil(int px, int py):Enemigo(px, py, 'M', GREEN, 1, 10){}
};
class EnemigoMedio : public Enemigo{
public:
	EnemigoMedio(): Enemigo(){}
	EnemigoMedio(int px, int py): Enemigo(px, py, 'W', YELLOW, 2, 20){}
};
class EnemigoFuerte: public Enemigo{
public:
	EnemigoFuerte(): Enemigo(){}
	EnemigoFuerte(int px, int py): Enemigo(px, py, 'H', RED, 3, 30){}
};
int main (int argc, char *argv[]) {
	
	
	
	
	
	
	
	
	
	
	return 0;
}

