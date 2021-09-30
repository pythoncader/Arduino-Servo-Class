#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int servomin = 125;
int servomax = 575;

class CServo {
  public:
    int channel;
    int currentAngle;
    bool infoPrint;
    bool glidingInfoPrint;

    void init(int channel, int currentAngle = -1, bool infoPrint = true) {
      (*this).channel = channel;
      (*this).currentAngle = currentAngle;
      (*this).infoPrint = infoPrint;
    }
    void print() {
      Serial.print("Servo on channel ");
      Serial.print((*this).channel);
      Serial.print(" at ");
      Serial.print((*this).currentAngle);
      Serial.println(" degrees");
    }
    void set_infoPrint(infoPrint) {
      (*this).infoPrint = infoPrint;
    }
    int angleToPulse(int ang) {
      int pulse = map(ang, 0, 180, servomin, servomax); // map angle of 0 to 180 to Servo min and Servo max
      return pulse;
    }
    void set_angle(int myangle = 90, float servowait = 0.3) {
      (*this).currentAngle = myangle;

      pwm.setPWM(channel, 0, angleToPulse(myangle));
      if ((*this).infoPrint == true) {
        Serial.print("Setting Servo on channel ");
        Serial.print((*this).channel);
        Serial.print(" to ");
        Serial.print((*this).currentAngle);
        Serial.print(" and waiting ");
        Serial.print(servowait);
        Serial.println(" seconds");
      }
      int mseconds = servowait * 1000;
      delay(mseconds);
    }
    void glide_angle(int startingAngle, int endingAngle, float timeToTake) {
      if ((*this).infoPrint == true) {
        (*this).infoPrint = false;
        (*this).glidingInfoPrint = true;

        Serial.print("Servo on channel ");
        Serial.print((*this).channel);
        Serial.print(" gliding from angle ");
        Serial.print(startingAngle);
        Serial.print(" to ");
        Serial.print(endingAngle);
        Serial.print(" in ");
        Serial.print(timeToTake);
        Serial.println(" seconds");
      } else {
        (*this).glidingInfoPrint = false;
      }
      int difference = startingAngle - endingAngle;
      int absdifference = abs(difference);
      float time_interval = timeToTake / absdifference;

      (*this).set_angle(startingAngle, 0.2);
      if (startingAngle > endingAngle) { //starting angle is 180 and ending angle is 0
        for (int i = startingAngle - 1; i >= endingAngle; i--) { //start at 179 and keep decrementing toward 0
          (*this).set_angle(i, time_interval);
        }
      } else {
        int neg_angle = startingAngle - 1; //starts at -1 if angle is 0
        while (neg_angle != endingAngle) {
          neg_angle++;
          (*this).set_angle(neg_angle, time_interval);
        }
      }
      if (glidingInfoPrint == true) {
        (*this).infoPrint = true;
      }
    }
};

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  Serial.println("my_servo stuff:");

  CServo my_servo;
  my_servo.init(0);
  my_servo.print();
  my_servo.set_angle(80, 5);
  my_servo.print();
  my_servo.set_angle(0, 0.2);
  my_servo.print();
  my_servo.glide_angle(0, 180, 5);
  my_servo.print();
  my_servo.glide_angle(180, 0, 2);
  my_servo.print();

  Serial.println("");
  Serial.println("cade_servo stuff:");

  CServo cade_servo;
  cade_servo.init(1);
  cade_servo.print();
  cade_servo.set_angle();
  cade_servo.print();
  cade_servo.glide_angle(0, 180, 5);
  cade_servo.print();
  cade_servo.glide_angle(180, 0, 2);
  cade_servo.print();

  delay(500);
  cade_servo.set_angle(0, 0);
  my_servo.set_angle(0, 0.3);

  cade_servo.set_angle(180, 0);
  my_servo.set_angle(180);

  CServo hey;
  hey.init(3);
  hey.print();
  hey.set_angle(0);

  CServo helloworld;
  helloworld.init(4, 0, false);
  helloworld.print();
  helloworld.set_angle(0);

  Serial.println("");
  Serial.println("Program ended.");
}
void loop() {
  delay(1000);
}
