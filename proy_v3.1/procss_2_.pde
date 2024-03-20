import processing.serial.*;
Serial miPuerto;
String textoIngresado = ""; //Variable para almacenar la entrada por teclado del usuario
String cadena=" ";
int estado=0; //Variable para los estados del sistema
boolean ingreseText= false; //Variable para el estado de ingreso de texto
boolean permitirEscritura = false;
int estad=0;
String datosRecibidos = ""; // Variable para almacenar los datos recibidos
boolean inicioRegistro = false; // Variable para controlar si se ha detectado la palabra "Registro"
String registro = "";

class RectanguloVisual {
  float x, y; // Posición del rectángulo
  float ancho, alto; // Dimensiones del rectángulo
  color colorFondo; // Color de fondo del rectángulo
  String texto; // Texto dentro del rectángulo
  int tamTexto;

  // Constructor de la clase
  RectanguloVisual(float x, float y, float ancho, float alto, color colorFondo, String texto, int tamTexto) {
    this.x = x;
    this.y = y;
    this.ancho = ancho;
    this.alto = alto;
    this.colorFondo = colorFondo;
    this.texto = texto;
    this.tamTexto = tamTexto;
  }

  // Método para mostrar el rectángulo visualmente
  void mostrar() {
    fill(colorFondo);
    rect(x, y, ancho, alto);
    fill(0);
    textAlign(CENTER, CENTER);
    textSize(tamTexto);
    text(texto, x + ancho/2, y + alto/2);
    textAlign(LEFT);
  }
}

RectanguloVisual rectangulo1 = new RectanguloVisual(100, 120, 400, 100, color(150, 50, 50), "Agregar Persona", 30);
RectanguloVisual rectangulo1a = new RectanguloVisual(100, 120, 400, 100, color(200, 50, 50), "Agregar Persona", 30);
RectanguloVisual rectangulo2 = new RectanguloVisual(100, 240, 400, 100, color(150, 50, 50), "Eliminar Persona", 30);
RectanguloVisual rectangulo2a = new RectanguloVisual(100, 240, 400, 100, color(200, 50, 50), "Eliminar Persona", 30);
RectanguloVisual rectangulo3 = new RectanguloVisual(100, 360, 400, 100, color(150, 50, 50), "Mostrar el Registro", 30);
RectanguloVisual rectangulo3a = new RectanguloVisual(100, 360, 400, 100, color(200, 50, 50), "Mostrar el Registro", 30);
RectanguloVisual rectanguloVolv = new RectanguloVisual(400, 415, 100, 40, color(120, 50, 50), "Volver", 20);
RectanguloVisual rectanguloVolva = new RectanguloVisual(400, 415, 100, 40, color(200, 50, 50), "Volver", 20);

void setup() {
  size(600, 500);
  background(180, 30, 30);
  miPuerto=new Serial(this, "COM4", 9600);
  fill(0);
  textSize(35);
  text("Guardia de BV El Libertador San Martín", width/2-280, 50);
  //line(width/2,0,width/2,800);
}

void draw() {

  switch(estado) {
  case 0: //estado inicial del programa

    fill(150, 10, 20);
    noStroke();
    rect(50, 80, 500, 450);
    rectangulo1.mostrar();
    if (mouseX>100 && mouseX<500 && mouseY>120 && mouseY<220) {
      rectangulo1a.mostrar();
      if (mousePressed) {
        estado=1;
        miPuerto.write("ingreso");
      }
    }

    //Segundo cuadrado de personal
    rectangulo2.mostrar();
    if (mouseX>100 && mouseX<500 && mouseY>240 && mouseY<340) {
      rectangulo2a.mostrar();
      if (mousePressed) {
        estado=2;
        miPuerto.write("eliminar");
      }
    }

    //Tercer cuadrado de personal
    rectangulo3.mostrar();
    if (mouseX>100 && mouseX<500 && mouseY>360 && mouseY<460) {
      rectangulo3a.mostrar();
      if (mousePressed) {
        estado=3;
        registro="";
        miPuerto.write("registro");
      }
    }
    break;

  case 1: //Agregar personal de guardia
    fill(150, 10, 20);
    noStroke();
    rect(50, 80, 500, 450);
    fill(0);
    textSize(30);
    text("Agregar personal de guardia", width/2-170, 130);
    rectBlan();
    volver();
    break;

  case 2: //Quitar personal de guardia
    fill(150, 10, 20);
    noStroke();
    rect(50, 80, 500, 450);
    fill(0);
    textSize(30);
    text("Quitar personal de guardia", width/2-170, 130);
    textSize(24);
    fill(0);
    text("Acerque la tarjeta que desea eliminar\n", 125, 295);
    textSize(17);
    text("\n(Aparecerá un mensaje en el LCD de cuando se elimine)", 115, 295);
    volver();
    break;

  case 3: //Mostrar personal de guardia
    fill(150, 10, 20);
    noStroke();
    rect(50, 80, 500, 450);
    fill(0);
    textSize(30);

    text("Mostrar personal de guardia", width/2-170, 130);
    /*while (miPuerto.available() == 0) {
     delay(150);
     }
     */
    while (miPuerto.available() > 0) {
      registro += miPuerto.readString();
    }
    fill(0);
    textSize(15);
    text(registro, 120, 200);
    volver();
    break;
  }
}

void volver() {
  rectanguloVolv.mostrar();
  if (mouseX>400 && mouseX<500 && mouseY>415 && mouseY<455) {
    rectanguloVolva.mostrar();
    if (mousePressed) {
      estado=0;
      estad=0;
      delay(200);
    }
  }
}

void rectBlan() {  //posible mejora con estados tmb

  char tecla = key;
  textSize(20);
  fill(0);
  text("Ingrese el nombre de la persona", 165, 240);
  text("(Solo apellido y nombre)", 195, 270);
  fill(255);
  rect(150, 300, 300, 40);
  fill(150);
  text("Ejemplo: Fernandez Javier", 185, 325);

  switch(estad) {
  case 0:
    if (mouseX>150 && mouseX<450 && mouseY>300 && mouseY<340) {
      fill(185);
      rect(150, 300, 300, 40);
      fill(150);
      text("Ejemplo: Fernandez Javier", 185, 325);
      if (mousePressed) {
        estad=1;
      }
    }
    break;
  case 1:
    fill(185);
    rect(150, 300, 300, 40);
    permitirEscritura = true;
    fill(0);
    text(textoIngresado, 185, 325);
    break;
  }
}

void keyTyped() {
  if (permitirEscritura) {
    char tecla = key;
    if (Character.isLetter(tecla) || tecla == ' ') {
      textoIngresado = textoIngresado + tecla;
    } else if (key == BACKSPACE && textoIngresado.length() > 0) {
      textoIngresado = textoIngresado.substring(0, textoIngresado.length() - 1);
    } else if (key == ENTER || key == RETURN) {
      // Finalizar la entrada de texto cuando se presiona Enter
      if (!textoIngresado.equals("")) {
        miPuerto.write(textoIngresado);
      }
      textoIngresado = ""; // Limpiar el texto ingresado
      permitirEscritura = false; // Restablecer la variable de estado
      estad=0;
    }
  }
}
void fuente() {
  textSize(25);
  fill(0);
}
