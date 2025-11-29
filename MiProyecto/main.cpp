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

class Jugador: public ObjetoJuego{
private:
	int vidas;
	int puntos;
	
public:
	Jugador(): ObjetoJuego(ANCHO_PANTALLA/2, ALTO_PANTALLA - 3, 'A', LIGHTBLUE), vidas(3), puntos(0){}
	
	void mover() override{
		if (kbhit()){
			int tecla = getch();
			borrar();
			
			if(tecla == 75 && x > 2){
				x--;
			}
			else if(tecla == 77 && x < ANCHO_PANTALLA - 2){
				x++;
			}
			dibujar();
		}
	}
	
	void recibirDanio(){
		vidas--;
		for(int i = 0; i < 3; i++){
			borrar();
			Sleep(100);
			dibujar();
			Sleep(100);
		}
	}
	void sumarPuntos(int p) {puntos += p;}
	int getVidas(){return vidas;}
	int getPuntos(){return puntos;}
};

class Juego{
private:
	Jugador jugador;
	Enemigo enemigos[MAX_ENEMIGOS];
	Bala balasJugador[MAX_BALAS_JUGADOR];
	Bala balasEnemigas[MAX_BALAS_ENEMIGAS];
	int numEnemigos;
	int direccionEnemigos;
	int contadorMovimiento;
	bool juegoActivo;
	
public:
	Juego() {
		srand(time(NULL));
		direccionEnemigos = 1;
		contadorMovimiento = 0;
		juegoActivo = true;
		numEnemigos = 0;
		inicializarEnemigos();
	}
	
	void inicializarEnemigos() {
		int filas = 4;
		int columnas = 10;
		int espacioX = 6;
		int espacioY = 2;
		int inicioX = 10;
		int inicioY = 4;
		numEnemigos = 0;
		
		for(int f = 0;f < filas; f++){
			for(int c = 0; c < columnas; c++){
				int px = inicioX + c * espacioX;
				int py = inicioY + f * espacioY;
				if (f == 0){
					enemigos[numEnemigos] = EnemigoFuerte(px, py);
				} else if (f <= 2){
					enemigos[numEnemigos] = EnemigoMedio(px, py);
				} else {
					enemigos[numEnemigos] = EnemigoDebil(px, py);
				}
				numEnemigos++;
			}
		}
	}
	
	void dibujarMarco(){
		textcolor(LIGHTGRAY);
		for (int i = 1; i <= ANCHO_PANTALLA; i++){
			gotoxy(i, 1); cprintf("-");
			gotoxy(i, ALTO_PANTALLA - 1);
			cprintf("-");
		}
		for(int i = 1;i <= ALTO_PANTALLA; i++){
			gotoxy(1, i); cprintf("|");
			gotoxy(ANCHO_PANTALLA, i);
			cprintf("|");
		}
	}
	
	void dibujarHUD() {
		textcolor(WHITE);
		gotoxy(3, ALTO_PANTALLA);
		cprintf("Vidas: %d  Puntos: %d Enemigos: %d", jugador.getVidas(), jugador.getPuntos(), contarEnemigosActivos());
	}
	
	int contarEnemigosActivos(){
		int contador = 0;
		for (int i = 0; i < numEnemigos; i++) {
			if (enemigos[i].estaActivo()) contador++;
		}
		return contador;
	}
	
	void moverEnemigos() {
		contadorMovimiento++;
		if (contadorMovimiento < 15) return;
		contadorMovimiento = 0;
		
		bool cambiarDireccion = false;
		
		for (int i = 0; i < numEnemigos; i++) {
			if (!enemigos[i]. estaActivo()) continue;
			
			if ((direccionEnemigos == 1 && enemigos[i].getX() >= ANCHO_PANTALLA - 3) || (direccionEnemigos == -1 && enemigos[i].getX() <= 3)) {
				cambiarDireccion = true;
				break;
			}
		}
		
		if (cambiarDireccion) {
			direccionEnemigos *= -1;
		}
		
		for (int i = 0; i < numEnemigos; i++) {
			if (!enemigos[i].estaActivo()) continue;
			
			int nuevaX = enemigos[i].getX() + direccionEnemigos;
			int nuevaY = enemigos[i].getY();
			
			if (cambiarDireccion) {
				nuevaY++;
			}
			
			enemigos[i].moverPosicion(nuevaX, nuevaY);
		}
	}
	
	void procesarDisparo() {
		if (kbhit()){
			int tecla = getch();
			if (tecla == ' ') {
				for (int i = 0; i < MAX_BALAS_JUGADOR; i++) {
					if (!balasJugador[i].estaActivo()) {
						balasJugador[i] = Bala(jugador.getX(), jugador.getY() -1, -1);
						break;
					}
				}
			}
		}
		
	}
	
	void enemigosDisparan() {
		if (rand() % 50 == 0) {
			for (int i = 0; i < MAX_BALAS_ENEMIGAS; i++) {
				if (!balasEnemigas[i].estaActivo()) {
					int indiceAleatorio = rand() % numEnemigos;
					if (enemigos[indiceAleatorio].estaActivo()) {
						balasEnemigas[i] = Bala(enemigos[indiceAleatorio].getX(), enemigos[indiceAleatorio].getY() + 1, 1);
					}
					break;
				}
			}
		}
	}
	
	void verificarColisiones() {
		for (int i = 0; i < MAX_BALAS_JUGADOR; i++) {
			if (!balasJugador[i].estaActivo()) continue;
			
			for (int j = 0; j < numEnemigos; j++){
				if (!enemigos[j].estaActivo()) continue;
				
				if (balasJugador[i].getX() == enemigos[j].getX() && balasJugador[i].getY() == enemigos[j].getY()) {
					balasJugador[i].desactivar();
					balasJugador[i].borrar();
					enemigos[j].recibirDanio();
					
					if (!enemigos[j].estaActivo()) {
						enemigos[j].borrar();
						jugador.sumarPuntos(enemigos[j].getPuntos());
					}
					break;
				}
			}
		}
		
		for (int i = 0; i < MAX_BALAS_ENEMIGAS; i++) {
			if (!balasEnemigas[i].getX() == jugador.getX() && balasEnemigas[i].get() == jugador.getY()) {
				balasEnemigas[i].desactivar();
				balasEnemigas[i].borrar();
				jugador.recibirDanio();
			}
		}
	}
	
	bool verificarCondicionesFinales() {
		if (contarEnemigosActivos() == 0) {
			clrscr();
			textcolor(GREEN);
			gotoxy(ANCHO_PANTALLA/2 - 10, ALTO_PANTALLA/2);
			cprintf("victoria");
			gotoxy(ANCHO_PANTALLA/2 - 15, ALTO_PANTALLA/2 +2);
			cprintf("Puntos finales: %d", jugador.getPuntos());
			return true;
		}
		
		if (jugador.getVidas() <= 0) {
			clrscr();
			textcolor(RED);
			gotoxy(ANCHO_PANTALLA/2 - 10, ALTO_PANTALLA/2);
            cprintf("Game over");
			gotoxy(ANCHO_PANTALLA/2 - 15, ALTO_PANTALLA/2 + 2);
			cprintf("Puntos finales: %d", jugador.getPuntos());
			return true;
		}
		
		for (int i = 0; i < numEnemigos; i++) {
			if (enemigos[i].estaActivo() && enemigos[i].getY() >= jugador.getY() - 1) {
				clrscr();
				textcolor(RED);
				gotoxy(ANCHO_PANTALLA/2 - 10, ALTO_PANTALLA/2);
				cprintf("Invadido por alienigenas");
				return true:
			}
		}
		
		return false;
	}
	
	void ejecutar(){
		clrscr();
		dibujarMarco();
		jugador.dibujar();
		
		for (int i = 0; i < numEnemigos; i++) {
			enemigos[i].dibujar();
		}
		
		while (juegoActivo) {
			jugador.mover();
			procesarDisparo();
			moverEnemigos();
			enemigosDisparan();
			
			for (int i = 0; i < MAX_BALAS_JUGADOR; i++) {
				if (balasJugador[i].estaActivo()) balasJugador[i].mover();
			}
			for (int i = 0; i < MAX_BALAS_ENEMIGAS; i++) {
				if (balasEnemigas[i].estaActivo()) balasEnemigas[i].mover();
			}
			
			verificarColisiones();
			dibujarHUD();
			
			if (verificarCondicionesFinales()) {
				juegoActivo = false;
			}
			
			Sleep(50);
		}
		
		gotoxy(1, ALTO_PANTALLA + 2);
		cprintf("Presiona cualquier tecla para salir...");
		getch;
	}
};
int main (int argc, char *argv[]) {
	
	
	
	
	
	
	
	
	
	
	return 0;
}

