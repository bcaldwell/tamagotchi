import processing.serial.*;

PImage bg;
PImage normal;
int imageScale = 4;
int ychange = 0;
int xchange = 0;
boolean movingUp = true;
boolean movingLeft = true;

int ychangeSpeed = 3;
int xchangeSpeed = 4;
int rotateSpeed = 5;

String mode = "move";

int rotateDegree = 0;

public void setup() {
  size (1152, 504);
  bg = loadImage("../background.jpg");
  normal = loadImage("../normal_face.png");
  normal.resize(normal.width/imageScale, normal.height/imageScale);
  imageMode(CENTER);
}


public void draw() {  
  background(bg);  
  translate(width/2 - xchange, height/2 - ychange);

  if (mode == "move") {
    imgMove();
  } else if (mode == "rotate") {
    imgRotate();
  }

  image(normal, 0, 0);
}

void mouseClicked() {
  if (mode == "move") {
    mode = "rotate";
  }
}

void imgRotate () {
  rotate(rotateDegree*TWO_PI/360);
  if (rotateDegree >= 360){
    mode = "move";
    rotateDegree = 0;
  }
  rotateDegree += rotateSpeed;
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
  rotate(rotateDegree*TWO_PI/360);
}

