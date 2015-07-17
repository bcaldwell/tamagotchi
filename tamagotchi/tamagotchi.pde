import processing.serial.*;

PImage bg;
PImage normal;
PImage steak;
PImage angry;

int imageScale = 4;
int ychange = 0;
int xchange = 0;
boolean movingUp = true;
boolean movingLeft = true;

int eaten = 20;

int ychangeSpeed = 3;
int xchangeSpeed = 4;
int rotateSpeed = 5;
int eatSpeed = 3;

String mode = "eating";
String faceState = "normal";

int rotateDegree = 0;

public void setup() {
  size (800, 800);
  bg = loadImage("../background.png");
  normal = loadImage("../normal_face.png");
  angry = loadImage("../angry_face.png");
  normal.resize(normal.width/imageScale, normal.height/imageScale);
  angry.resize(angry.width/imageScale, angry.height/imageScale);

  steak = loadImage("../steak.gif");
  steak.resize(steak.width/2, steak.width/2);
  imageMode(CENTER);
}


public void draw() {  
  background(bg); 

  if (mode == "move") {
    imgMove();
  } else if (mode == "rotate") {
    imgRotate();
  } else if (mode == "jumpGame") {
    jumpingGame();
  } else if (mode == "eating") {
    eating();
  }

  translate(width/2 - xchange, height/2 - ychange);
  rotate(rotateDegree*TWO_PI/360);
  if (faceState == "angry") {
    image(angry, 0, 0);
  } else {
    image(normal, 0, 0);
  }
}

void mouseClicked() {
//  if (mode == "move") {
//    mode = "eating";
//  }
  if (faceState == "angry") {
    faceState = "normal";
  } else {
    faceState = "angry";
  }
}

void imgRotate () {
  rotateDegree += rotateSpeed;
  if (rotateDegree >= 360) {
    mode = "move";
    rotateDegree = 0;
  }
}

void imgMove () {
  //determine y position
  if (movingUp) {
    ychange+= ychangeSpeed;
  } else {
    ychange-= ychangeSpeed;
  }
  if (ychange >= 75) {
    movingUp = false;
  } else if (ychange <= 0) {
    movingUp = true;
    movingLeft = boolean(int(random(0, 100)%2));
  }

  if (movingLeft) {
    xchange+= xchangeSpeed;
  } else {
    xchange-= xchangeSpeed;
  }

  if (abs(xchange) >= (width - normal.width)/2) {

    movingLeft = !movingLeft;
  }
}

void eating() {  
  image(steak.get(0, 0, steak.width, steak.height - eaten), 200, 200);
  eaten += eatSpeed;
  eaten = min(eaten, steak.height);
  if (eaten == steak.height) {
    eaten = 0;
    mode = "move";
  }
}

void jumpingGame() {
  rectMode(CENTER);
  noFill();
  strokeWeight(20);
  rect(0, 0, 200, 0);
}

