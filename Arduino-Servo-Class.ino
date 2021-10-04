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
    void random_angle(float myrandom_time = 200) {
      float random_time = (random(0, myrandom_time)) / 1000.0;
      (*this).set_angle(random(0, 180), random_time);
    }
    void vibrate(int interval = 15, float delaytime = 3, int duration = 100) {
      Serial.println("vibrate starting...");
      int starttime = millis();
      int endtime = starttime;
      int duration_millis = duration * 1000;
      for (int i = interval; i <= 180; i = i + interval) {
        (*this).set_angle(0, 0);
        delay(delaytime * 1000);
        (*this).set_angle(i, 0);
        delay(delaytime * 1000);
        if ((endtime - starttime) <= duration_millis){
          endtime = millis();
        }else {
          break;
        }
      }
      (*this).set_angle(0, 0);
      Serial.println("vibrate ending...");
    }
};

class CServoGroup {
  public:
    int currentAngle;
    bool infoPrint;
    int num_of_servos;
    bool glidingInfoPrint;
    CServo servo0;
    CServo servo1;
    CServo servo2;
    CServo servo3;
    CServo servo4;
    CServo servo5;
    CServo servo6;
    CServo servo7;
    void init(CServo servo0, CServo servo1, CServo servo2, CServo servo3, CServo servo4, CServo servo5, CServo servo6, CServo servo7, int currentAngle = -1, bool infoPrint = false) {
      (*this).currentAngle = currentAngle;
      (*this).infoPrint = infoPrint;
      (*this).servo0 = servo0;
      (*this).servo1 = servo1;
      (*this).servo2 = servo2;
      (*this).servo3 = servo3;
      (*this).servo4 = servo4;
      (*this).servo5 = servo5;
      (*this).servo6 = servo6;
      (*this).servo7 = servo7;
    }
    void set_angle(int myangle = 90, float wait_time = 1) {
      servo0.set_angle(myangle, 0);
      servo1.set_angle(myangle, 0);
      servo2.set_angle(myangle, 0);
      servo3.set_angle(myangle, 0);
      servo4.set_angle(myangle, 0);
      servo5.set_angle(myangle, 0);
      servo6.set_angle(myangle, 0);
      servo7.set_angle(myangle, 0);
      delay(wait_time * 1000);
    }
    void glide_angle(int startingAngle, int endingAngle, float timeToTake) {
      if ((*this).infoPrint == true) {
        (*this).infoPrint = false;
        (*this).glidingInfoPrint = true;

        Serial.print("ServoGroup gliding from angle ");
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
class ServoPumpkin {
  public:
    CServo eye0;
    CServo eye1;
    CServo eye2;
    CServo eye3;
    CServo eye4;
    CServo eye5;
    CServo eye6;
    CServo eye7;

    void init(CServo eye0, CServo eye1, CServo eye2, CServo eye3, CServo eye4, CServo eye5, CServo eye6, CServo eye7) {
      (*this).eye0 = eye0;
      (*this).eye1 = eye1;
      (*this).eye2 = eye2;
      (*this).eye3 = eye3;
      (*this).eye4 = eye4;
      (*this).eye5 = eye5;
      (*this).eye6 = eye6;
      (*this).eye7 = eye7;
    }
    void reset_out(float amount_time = 2) {
      eye0.set_angle(180, 0);
      eye1.set_angle(180, 0);
      eye2.set_angle(180, 0);
      eye3.set_angle(0, 0);
      eye4.set_angle(180, 0);
      eye5.set_angle(0, 0);
      eye6.set_angle(0, 0);
      eye7.set_angle(0, 0);
      delay(amount_time * 1000);
    }
    void random_eyes(int duration, int random_time = 200) { //give duration of running in seconds
      Serial.println("pumpkin random starting...");
      int starttime = millis();
      int endtime = starttime;
      int duration_millis = duration * 1000;
      while ((endtime - starttime) <= duration_millis) {
        eye0.random_angle(random_time);
        eye1.random_angle(random_time);
        eye2.random_angle(random_time);
        eye3.random_angle(random_time);
        eye4.random_angle(random_time);
        eye5.random_angle(random_time);
        eye6.random_angle(random_time);
        eye7.random_angle(random_time);

        endtime = millis();
      }
      Serial.println("pumpkin random ending...");
    }
    void min_max(int duration, int delay_amount = 1) { //give duration of running in seconds
      Serial.println("pumpkin min_max starting...");
      int starttime = millis();
      int endtime = starttime;
      int duration_millis = duration * 1000;
      while ((endtime - starttime) <= duration_millis) {
        delay(delay_amount * 1000);
        eye0.set_angle(0, 0);
        eye1.set_angle(0, 0);
        eye2.set_angle(0, 0);
        eye3.set_angle(180, 0);
        eye4.set_angle(0, 0);
        eye5.set_angle(180, 0);
        eye6.set_angle(180, 0);
        eye7.set_angle(180, 0);

        delay(delay_amount * 1000);

        eye0.set_angle(180, 0);
        eye1.set_angle(180, 0);
        eye2.set_angle(180, 0);
        eye3.set_angle(0, 0);
        eye4.set_angle(180, 0);
        eye5.set_angle(0, 0);
        eye6.set_angle(0, 0);
        eye7.set_angle(0, 0);

        endtime = millis();
      }
      Serial.println("pumpkin min_max ending...");
    }
    void min_max_glide(float eye_speed, int delay_amount = 1) {
      (*this).reset_out(4);
      Serial.println("pumpkin min_max_glide starting...");

      eye0.glide_angle(180, 0, eye_speed);
      eye1.glide_angle(180, 0, eye_speed);
      eye2.glide_angle(180, 0, eye_speed);
      eye3.glide_angle(0, 180, eye_speed);
      eye7.glide_angle(0, 180, eye_speed);
      eye6.glide_angle(0, 180, eye_speed);
      eye5.glide_angle(0, 180, eye_speed);
      eye4.glide_angle(180, 0, eye_speed);

      delay(delay_amount * 1000);
      eye4.glide_angle(0, 180, eye_speed);
      eye5.glide_angle(180, 0, eye_speed);
      eye6.glide_angle(180, 0, eye_speed);
      eye7.glide_angle(180, 0, eye_speed);
      eye3.glide_angle(180, 0, eye_speed);
      eye2.glide_angle(0, 180, eye_speed);
      eye1.glide_angle(0, 180, eye_speed);
      eye0.glide_angle(0, 180, eye_speed);

      Serial.println("pumpkin min_max_glide ending...");
    }
    void half_half(int delay_amount = 1) {
      (*this).reset_out();
      Serial.println("Half_Half starting...");

      //start half half
      eye0.set_angle(0, 0);
      eye1.set_angle(0, 0);
      eye4.set_angle(0, 0);
      eye5.set_angle(180, 0);

      delay(delay_amount * 1000);

      eye0.set_angle(180, 0);
      eye1.set_angle(180, 0);
      eye4.set_angle(180, 0);
      eye5.set_angle(0, 0);

      delay(delay_amount * 1000);
      eye2.set_angle(0, 0);
      eye3.set_angle(180, 0);
      eye6.set_angle(180, 0);
      eye7.set_angle(180, 0);

      delay(delay_amount * 1000);

      eye2.set_angle(180, 0);
      eye3.set_angle(0, 0);
      eye6.set_angle(0, 0);
      eye7.set_angle(0, 0);

      Serial.println("Half_Half ending...");
    }
    void columns(int delay_amount = 1) {
      (*this).reset_out();
      Serial.println("columns starting...");
      //start each column is a group

      //column 1:
      eye0.set_angle(0, 0);
      eye4.set_angle(0, 0);
      delay(delay_amount * 1000);
      eye0.set_angle(180, 0);
      eye4.set_angle(180, 0);
      delay(delay_amount * 1000);

      //column 2:
      eye1.set_angle(0, 0);
      eye5.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye1.set_angle(180, 0);
      eye5.set_angle(0, 0);
      delay(delay_amount * 1000);

      //column 3:
      eye2.set_angle(0, 0);
      eye6.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye2.set_angle(180, 0);
      eye6.set_angle(0, 0);
      delay(delay_amount * 1000);

      //column 4:
      eye3.set_angle(180, 0);
      eye7.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye3.set_angle(0, 0);
      eye7.set_angle(0, 0);
      delay(delay_amount * 1000);

      Serial.println("columns ending...");
    }
    void columns_converging(int delay_amount = 1) {
      (*this).reset_out();
      Serial.println("columns converging starting...");
      //start each column is a group

      //column 1:
      eye0.set_angle(0, 0);
      eye4.set_angle(0, 0);
      //column 4:
      eye3.set_angle(180, 0);
      eye7.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye0.set_angle(180, 0);
      eye4.set_angle(180, 0);
      eye3.set_angle(0, 0);
      eye7.set_angle(0, 0);
      delay(delay_amount * 1000);

      //column 2:
      eye1.set_angle(0, 0);
      eye5.set_angle(180, 0);
      //column 3:
      eye2.set_angle(0, 0);
      eye6.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye1.set_angle(180, 0);
      eye5.set_angle(0, 0);
      eye2.set_angle(180, 0);
      eye6.set_angle(0, 0);
      delay(delay_amount * 1000);

      Serial.println("columns converging ending...");
    }
    void rows(int delay_amount = 1) {
      (*this).reset_out();
      Serial.println("rows starting...");
      //start each row is a group

      //top row:
      eye0.set_angle(0, 0);
      eye1.set_angle(0, 0);
      eye2.set_angle(0, 0);
      eye3.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye0.set_angle(180, 0);
      eye1.set_angle(180, 0);
      eye2.set_angle(180, 0);
      eye3.set_angle(0, 0);
      delay(delay_amount * 1000);

      //bottom row:
      eye4.set_angle(0, 0);
      eye5.set_angle(180, 0);
      eye6.set_angle(180, 0);
      eye7.set_angle(180, 0);
      delay(delay_amount * 1000);
      eye4.set_angle(180, 0);
      eye5.set_angle(0, 0);
      eye6.set_angle(0, 0);
      eye7.set_angle(0, 0);
      delay(delay_amount * 1000);
      Serial.println("rows ending...");
    }
    void lookdirections(int delay_amount = 1) {
      (*this).reset_out();
      Serial.println("lookdirections starting...");
      //start bottom halves at same time
      eye4.set_angle(0, 0); //inward
      eye5.set_angle(0, 0); //inward
      eye6.set_angle(0, 0); //outward
      eye7.set_angle(0, 0); //outward
      delay(delay_amount * 1000);
      eye4.set_angle(180, 0); //outward
      eye5.set_angle(180, 0); //outward
      eye6.set_angle(180, 0); //inward
      eye7.set_angle(180, 0); //inward
      delay(delay_amount * 1000);

      //start top halves at same time
      eye0.set_angle(0, 0); //inward
      eye1.set_angle(0, 0); //inward
      eye2.set_angle(180, 0); //outward
      eye3.set_angle(180, 0); //outward
      delay(delay_amount * 1000);
      eye0.set_angle(180, 0); //outward
      eye1.set_angle(180, 0); //outward
      eye2.set_angle(0, 0); //inward
      eye3.set_angle(0, 0); //inward
      delay(delay_amount * 1000);
      Serial.println("lookdirections ending...");
    }
    void ladders(int interval = 15, float delaytime = 3, int duration = 100) {
      Serial.println("ladders starting...");
      int starttime = millis();
      int endtime = starttime;
      int duration_millis = duration * 1000;
      for (int i = interval; i <= 180; i = i + interval) {
        eye0.set_angle(0, 0);
        eye1.set_angle(0, 0);
        eye2.set_angle(0, 0);
        eye3.set_angle(180, 0);
        eye4.set_angle(0, 0);
        eye5.set_angle(0, 0);
        eye6.set_angle(0, 0);
        eye7.set_angle(0, 0);
        delay(delaytime * 1000);
        eye0.set_angle(i, 0);
        eye1.set_angle(i, 0);
        eye2.set_angle(i, 0);
        eye3.set_angle(abs(i - 180), 0);
        eye4.set_angle(i, 0);
        eye5.set_angle(i, 0);
        eye6.set_angle(i, 0);
        eye7.set_angle(i, 0);
        delay(delaytime * 1000);
        if ((endtime - starttime) <= duration_millis){
          endtime = millis();
        }else {
          break;
        }
      }
      eye0.set_angle(0, 0);
      eye1.set_angle(0, 0);
      eye2.set_angle(0, 0);
      eye3.set_angle(180, 0);
      eye4.set_angle(0, 0);
      eye5.set_angle(0, 0);
      eye6.set_angle(0, 0);
      eye7.set_angle(0, 0);
      Serial.println("ladders ending...");
    }
    void vibrate_rounds(){
      eye0.vibrate(1, 0.05, 1);
      eye1.vibrate(1, 0.05, 1);
      eye2.vibrate(1, 0.05, 1);
      eye3.vibrate(1, 0.05, 1);
      eye4.vibrate(1, 0.05, 1);
      eye5.vibrate(1, 0.05, 1);
      eye6.vibrate(1, 0.05, 1);
      eye7.vibrate(1, 0.05, 1);
    }
};


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
  ServoPumpkin pumpkin;

  top_left.init(0, 35, 82);
  top_mid_left.init(1, 137, 180);
  top_mid_right.init(2, 35, 84);
  top_right.init(3, 80, 130);
  bottom_left.init(4, 30, 70);
  bottom_mid_left.init(5, 90, 140);
  bottom_mid_right.init(6, 80, 150);
  bottom_right.init(7, 38, 83);
  pumpkin.init(top_left, top_mid_left, top_mid_right, top_right, bottom_left, bottom_mid_left, bottom_mid_right, bottom_right);

  CServoGroup eyes;
  eyes.init(top_left, top_mid_left, top_mid_right, top_right, bottom_left, bottom_mid_left, bottom_mid_right, bottom_right);

  while (true) {
    pumpkin.vibrate_rounds();
    pumpkin.random_eyes(15);
    pumpkin.ladders(1, 0.05);
    pumpkin.min_max(6, 1);
    pumpkin.min_max_glide(0.25);
    pumpkin.rows();
    pumpkin.half_half();
    pumpkin.min_max_glide(0.1);
    pumpkin.lookdirections();
    pumpkin.ladders(10, 0.3);
    pumpkin.columns();
    eyes.set_angle(180);
    eyes.glide_angle(0, 180, 4);
    pumpkin.ladders(2, 0.1);
  }

}
void loop() {
  delay(1000);
}
