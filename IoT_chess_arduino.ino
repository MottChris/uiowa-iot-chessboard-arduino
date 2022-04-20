// Include the AccelStepper library:
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Button.h>


#include <Servo.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define x_dirPin 2
#define x_stepPin 3
#define x_motorInterfaceType 1

#define y_dirPin 4
#define y_stepPin 5
#define y_motorInterfaceType 1

#define servo_pot_pin 9

#define x_lim_switch 12
#define y_lim_switch 13


// Create a new instance of the AccelStepper class:
AccelStepper x_stepper = AccelStepper(x_motorInterfaceType, x_stepPin, x_dirPin);
AccelStepper y_stepper = AccelStepper(y_motorInterfaceType, y_stepPin, y_dirPin);

MultiStepper steppers;

Servo magnet_servo;

//Button x_button(12); // Connect your button to pin
//Button y_button(13); //

String inputString, x_coord, y_coord;


void setup() {
  Serial.begin(9600);
  /*
  x_button.begin();
  y_button.begin();
  */
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  magnet_servo.attach(9);
  magnet_servo.writeMicroseconds(815);
  
  // Set the maximum speed in steps per second:
  x_stepper.setMaxSpeed(200);
  y_stepper.setMaxSpeed(200);

  // Set the starting position to zero TODO: eventually we will want to do custom calibration
  //x_stepper.setCurrentPosition(0);

  
  //y_stepper.setCurrentPosition(0);
  
  steppers.addStepper(x_stepper);
  steppers.addStepper(y_stepper);

  
  calibrateMotors();

  long start_location[2]; // Array of desired stepper positions
  
  start_location[0] = 200;
  start_location[1] = 50;

  steppers.moveTo(start_location); 
  steppers.runSpeedToPosition(); // blocks until all are in position

  // init servo functionality
  
  // down magnet_servo.writeMicroseconds(500)
  // up magnet_servo.writeMicroseconds(815)

  Serial.println("Board Menu");
  Serial.println("----------");
  Serial.println("1. Move");
}

void calibrateMotors(){
  // calibrate x motor
  Serial.println("Calibrating");

  //x_stepper.setSpeed(-70);
  //x_stepper.runSpeed();
  
  while(digitalRead(12) == LOW){
    x_stepper.setSpeed(-90);
    x_stepper.runSpeed();
  }
  x_stepper.stop();
  x_stepper.setCurrentPosition(0);
  
  delay(500);
  //y_stepper.setSpeed(-70);
  //y_stepper.runSpeed();
  while(digitalRead(13) == LOW){
    y_stepper.setSpeed(-90);
    y_stepper.runSpeed();
    }
  y_stepper.stop();
  y_stepper.setCurrentPosition(0);
  delay(500);

  Serial.println("END Calibrating");
}
void loop() {
  Serial.println("Enter functionality");
  
  while(Serial.available() == 0) // constantly check to see if anything has been send to serial
  {}
  inputString = Serial.readString(); // read the input string from serial monitor
  

  if(inputString.length() > 0) {
    Serial.println("Entered String...");
    Serial.println(inputString);

    // Assume for now the user inputs the desired position in 8 characters, i.e, to move to position x = 300, y = 1200, we'd type 03001200 into the serial monitor
    x_coord = inputString.substring(0,4); // get the first four characters
    y_coord = inputString.substring(4,8); // get the last four characters

    int x_move = x_coord.toInt();
    int y_move = y_coord.toInt();

    // before table movement, we we want to put the servo up to initiate the magnet
    magnet_servo.writeMicroseconds(815);

    moveMotors(x_move, y_move);

    Serial.println("END MOVE MOTORS FUNCTION");
    magnet_servo.writeMicroseconds(1030);
    inputString = "";
  }
}

// Call this function from loop any time a new measurement is inputted into the serial monitor
void moveMotors(int x, int y) {
  long positions[2]; // Array of desired stepper positions

  positions[0] = x;
  positions[1] = y;

  steppers.moveTo(positions); 
  steppers.runSpeedToPosition(); // blocks until all are in position
  delay(1000);
}
