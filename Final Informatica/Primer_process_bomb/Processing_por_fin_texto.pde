import processing.serial.*;
Serial miPuerto;
String textoIngresado = ""; //Variable para almacenar la entrada por teclado del usuario
int estado=0; //Variable para los estados del sistema
boolean ingreseText= false; //Variable para el estado de ingreso de texto
boolean permitirEscritura = false;
int estad=0;

void setup() {
  size(600, 500);
  background(180, 30, 30);
  //miPuerto=new Serial(this, "COM6", 9600);
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

    //Primer cuadrado de personal
    fill(120, 50, 50);
    rect(100, 120, 400, 100);
    fuente();
    text("Agregar personal de guardia", 160, 180);
    if (mouseX>100 && mouseX<500 && mouseY>120 && mouseY<220) {
      fill(150, 50, 50);
      rect(100, 120, 400, 100);
      fuente();
      text("Agregar personal de guardia", 160, 180);
      if (mousePressed) {
        estado=1;
      }
    }

    //Segundo cuadrado de personal
    fill(120, 50, 50);
    rect(100, 240, 400, 100);
    fuente();
    text("Quitar personal de guardia", 160, 300);
    if (mouseX>100 && mouseX<500 && mouseY>240 && mouseY<340) {
      fill(150, 50, 50);
      rect(100, 240, 400, 100);
      fuente();
      text("Quitar personal de guardia", 160, 300);
      if (mousePressed) {
        estado=2;
      }
    }

    //Tercer cuadrado de personal
    fill(120, 50, 50);
    rect(100, 360, 400, 100);
    fuente();
    text("Ver el personal de guardia", 170, 420);
    if (mouseX>100 && mouseX<500 && mouseY>360 && mouseY<460) {
      fill(150, 50, 50);
      rect(100, 360, 400, 100);
      fuente();
      text("Ver el personal de guardia", 170, 420);
      if (mousePressed) {
        estado=3;
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
    rectBlan();
    volver();
    break;

  case 3: //Mostrar personal de guardia
    fill(150, 10, 20);
    noStroke();
    rect(50, 80, 500, 450);
    fill(0);
    textSize(30);
    text("Mostrar personal de guardia", width/2-170, 130);
    rectBlan();
    volver();
    break;
  }
}

void volver() {
  noStroke();
  fill(120, 50, 50);
  rect(400, 415, 100, 40);
  textSize(20);
  fill(0);
  text("Volver", 425, 442);
  if (mouseX>400 && mouseX<500 && mouseY>415 && mouseY<455) {
    fill(150, 50, 50);
    rect(400, 415, 100, 40);
    textSize(20);
    fill(0);
    text("Volver", 425, 442);
    if (mousePressed) {
      estado=0;
      estad=0;
      delay(100);
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
      // Aquí podrías realizar alguna acción con el texto ingresado, como imprimirlo
      if (!textoIngresado.equals("")) {
        println("Texto ingresado: " + textoIngresado);
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
