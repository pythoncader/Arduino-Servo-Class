#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

int servo_min = 120;
int servo_max = 650;

class CServo {
  public:
    int channel;
    int servo_min_bound;
    int servo_max_bound;
    bool infoPrint;
    bool glidingInfoPrint;
    int currentAngle = -1;

    void init(int channel, int servo_min_bound = 0, int servo_max_bound = 180, bool infoPrint = false) {
      (*this).channel = channel;
      (*this).infoPrint = infoPrint;
      (*this).servo_min_bound = servo_min_bound;
      (*this).servo_max_bound = servo_max_bound;

      if ((*this).infoPrint == true) {
        Serial.print("Initializing Servo on channel ");
        Serial.print((*this).channel);
        Serial.print(" with range ");
        Serial.print((*this).servo_min_bound);
        Serial.print(" to ");
        Serial.print((*this).servo_max_bound);
        Serial.println(" degrees.");
      }
    }
    void print() {
      Serial.print("Servo on channel ");
      Serial.print((*this).channel);
      Serial.print(" at ");
      Serial.print((*this).currentAngle);
      Serial.println(" degrees");
    }
    void set_infoPrint(bool infoPrint) {
      (*this).infoPrint = infoPrint;
    }
    int angleToPulse(int ang) {
      int pulse = map(ang, 0, 180, servo_min, servo_max); // map angle of 0 to 180 to Servo min and Servo max
      return pulse;
    }
    void set_angle(int myangle = 90, float servowait = 0.3) {
      (*this).currentAngle = myangle;

      if ((*this).servo_min_bound != 0 || (*this).servo_max_bound != 180) {
        myangle = map(myangle, 0, 180, (*this).servo_min_bound, (*this).servo_max_bound);
        (*this).currentAngle = myangle;
      }
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

void random_eye(CServo eye1, CServo eye2, CServo eye3, CServo eye4, CServo eye5, CServo eye6, CServo eye7, CServo eye8, int angle_difference = 30) {
  for (int random_channel = 0; random_channel < 8; random_channel++) {
    int my_random;
    switch (random_channel) {
      case 0:
        my_random = random(0, 180);
        while (abs(my_random - eye1.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye1.set_angle(my_random, 0);
      case 1:
        my_random = random(0, 180);
        while (abs(my_random - eye2.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye2.set_angle(my_random, 0);
      case 2:
        my_random = random(0, 180);
        while (abs(my_random - eye3.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye3.set_angle(my_random, 0);
      case 3:
        my_random = random(0, 180);
        while (abs(my_random - eye4.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye4.set_angle(my_random, 0);
      case 4:
        my_random = random(0, 180);
        while (abs(my_random - eye5.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye5.set_angle(my_random, 0);
      case 5:
        my_random = random(0, 180);
        while (abs(my_random - eye6.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye6.set_angle(my_random, 0);
      case 6:
        my_random = random(0, 180);
        while (abs(my_random - eye7.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye7.set_angle(my_random, 0);
      case 7:
        my_random = random(0, 180);
        while (abs(my_random - eye8.currentAngle) < angle_difference) {
          my_random = random(0, 180);
        }
        eye8.set_angle(my_random, 0);
    }
    delay(random(100, 500));
  }
}

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates

  CServo top_left;
  CServo top_mid_left;
  CServo top_mid_right;
  CServo top_right;
  CServo bottom_left;
  CServo bottom_mid_left;
  CServo bottom_mid_right;
  CServo bottom_right;

  top_left.init(0, 35, 82);
  top_mid_left.init(1, 137, 180);
  top_mid_right.init(2, 35, 84);
  top_right.init(3, 80, 135);
  bottom_left.init(4, 30, 70);
  bottom_mid_left.init(5, 90, 140);
  bottom_mid_right.init(6, 80, 150);
  bottom_right.init(7, 38, 83);

  while (true) {
    random_eye(top_left, top_mid_left, top_mid_right, top_right, bottom_left, bottom_mid_left, bottom_mid_right, bottom_right);
  }
  /*while (true) {
    delay(1000);
    top_left.set_angle(0, 0);
    top_mid_left.set_angle(180, 0);
    top_mid_right.set_angle(0, 0);
    top_right.set_angle(0, 0);
    bottom_left.set_angle(0, 0);
    bottom_mid_left.set_angle(0, 0);
    bottom_mid_right.set_angle(0, 0);
    bottom_right.set_angle(0, 0);

    delay(1000);

    bottom_right.set_angle(180, 0);
    top_left.set_angle(180, 0);
    top_mid_left.set_angle(0, 0);
    top_mid_right.set_angle(180, 0);
    top_right.set_angle(180, 0);
    bottom_left.set_angle(180, 0);
    bottom_mid_left.set_angle(180, 0);
    bottom_mid_right.set_angle(180, 0);
    }*/
}
void loop() {
  delay(1000);
}
