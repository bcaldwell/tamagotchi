import processing.serial.*;

Serial myPort;

boolean serialMode = true;

//these are the images
PImage bg;
PImage normal;
PImage steak;
PImage angry;
//variables
int imageScale = 4;
int ychange = 0;
int xchange = 0;
boolean movingUp = true;
boolean movingLeft = true;

int eaten = 20;
//speed variables
int ychangeSpeed = 3;
int xchangeSpeed = 4;
int rotateSpeed = 5;
int eatSpeed = 3;

String mode = "move";
String faceState = "normal";

int rotateDegree = 0;

float aliveTime = 0;
String feedTimes = "";
// load images
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

  //connect serial this is what connects arduino
  if (serialMode) {
    println (Serial.list());
    myPort = new Serial(this, Serial.list()[Serial.list().length - 1], 9600);
    myPort.bufferUntil('\n');
  }
}


public void draw() {  
  //decide what the action will be
  background(bg); 
  if (mode == "move") {
    imgMove();
  } else if (mode == "rotate") {
    imgRotate();
  } else if (mode == "eating") {
    eating();
  } else if (mode == "game") {
    game();
  } else if (mode == "dead") {
    dead();
  }

  translate(width/2 - xchange, height/2 - ychange);
  rotate(rotateDegree*TWO_PI/360);
  if (faceState == "angry") {
    image(angry, 0, 0);
  } else if (faceState == "normal") {
    image(normal, 0, 0);
  }
}

void dead() {
  textSize(50);
  fill(0);
  text("Time alive was: " + aliveTime + " seconds", 50, 50);
  text(feedTimes, 50, 250, width - 50, height);
}

//rotates image
void imgRotate () {
  rotateDegree += rotateSpeed;
  if (rotateDegree >= 360) {
    mode = "move";
    rotateDegree = 0;
  }
}
//Move around the screen
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
//load steak image and fade it out
void eating() {  
  image(steak.get(0, 0, steak.width, steak.height - eaten), 200, 200);
  eaten += eatSpeed;
  eaten = min(eaten, steak.height);
  if (eaten == steak.height) {
    eaten = 0;
    mode = "move";
  }
}

String val = "";
boolean firstContact = false;
void serialEvent( Serial myPort) {
  int button;
  int state;
  //put the incoming data into a String - 
  //the '\n' is our end delimiter indicating the end of a complete packet
  val = myPort.readStringUntil('\n');
  //make sure our data isn't empty before continuing
  if (val != null) {
    //trim whitespace and formatting characters (like carriage return)
    val = trim(val);

    //look for our 'A' string to  start the handshake
    //if it's there, clear the buffer, and send a request for data
    if (!firstContact) {
      if (val.equals("A")) {
        myPort.clear();
        firstContact = true;
        myPort.write("a");
        println("contact");
      }
    } else { //if we've already established contact, keep getting and parsing data
      println(val);
      try {
        if ((val.indexOf("Time alive was:")) >= 0) {
          aliveTime = float(val.substring(15));
          mode = "dead";
          faceState = "none";
        } else if (val.indexOf("You fed him at:") >= 0) {
          feedTimes = val;
          //          String vals = trim(val.substring(15));
          //          feedTimes = int(split (vals, " "));
          //          println(feedTimes);
        } else {
    //Logic for what buttons do to the tamogochi and sets state and modes
          button = int(val.substring(0, 1));
          state = int(val.substring(2, 3));
          if (mode != "game") {
            if (button == 0 && state == 1) {
              mode = "eating";
            } else if (button == 1) {
              faceState = (state == 1? "angry": "normal");
            } else if (button == 2 && state == 1) {
              mode ="game";
              faceState = "none";
            } else if (button == 3 && state == 0) {
              mode = "rotate";
            }
          } else {
            if (state == 1) {
              buttonPressed(button);
            }
          }
        }
        myPort.clear();
      }
      catch(RuntimeException e) {
        println(e);
      }
    }
  }
}

