#include <Stepper.h>
#include <Servo.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

//   u
// l f r b
//   d
 
// Size of rubik cube
const int N = 3;

Servo light_arm;

Servo move_arm;

Servo hold_arm;

Servo platform;

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int platform_rotation = 0;

// Delays
const int general_delay = 700;

void flip(){
  move_arm_back(700);
  
  // Orignally was 95
  move_arm.write(80);
  delay(1000);
  for(int i = 91; i <= 150; i++){
    move_arm.write(i);
    delay(15);
  }
  delay(700);
}

void move_arm_back(int mili_seconds){
  move_arm.write(150);
  delay(mili_seconds);
}

void hold(int mili_seconds){
  hold_arm.write(84);
  delay(mili_seconds);
}

void hold_arm_back(int mili_seconds){
  hold_arm.write(155);
  delay(mili_seconds);
}

void fix_cube(int mili_seconds){
  move_arm.write(115);
  delay(mili_seconds);
}

void platform_start(int mili_seconds){
  platform.write(12);
  platform_rotation = 0;
  delay(mili_seconds);
}

void platform_middle(int mili_seconds){
  platform.write(91);
  platform_rotation = 90;
  delay(mili_seconds);
}

void platform_end(int mili_seconds){
  platform.write(171);
  platform_rotation = 180;
  delay(mili_seconds);
}

void platform_clockwise(bool turning){
  if(platform_rotation == 0){
    if(turning){
      hold_arm_back(1500);
      platform_middle(1500);
      flip();
      flip();
      platform_end(1500);
      flip();
      flip();
      hold(1500);
      platform_middle(1500);
    }
    else{
      platform_middle(1500);
      flip();
      flip();
      platform_start(1500);
      flip();
      flip();
      platform_middle(1500);
    }
  }
  else if(platform_rotation == 90){
    platform_start(1500);
  }
  else if(platform_rotation == 180){
    platform_middle(1500);
  }
}

void platform_counterclockwise(bool turning){
  if(platform_rotation == 0){
    platform_middle(1500);
  }
  else if(platform_rotation == 90){
    platform_end(1500);
  }
  else if(platform_rotation == 180){
    if(turning){
      hold_arm_back(1500);
      platform_middle(1500);
      flip();
      flip();
      platform_start(1500);
      flip();
      flip();
      hold(1500);
      platform_middle(1500);
    }
    else{
      platform_middle(1500);
      flip();
      flip();
      platform_start(1500);
      flip();
      flip();
      platform_middle(1500);
    }
  }
}

void platform_turn_clockwise(){
  hold(general_delay);
  platform_clockwise(true);
  hold_arm_back(general_delay);
  fix_cube(general_delay);
  move_arm_back(general_delay);
}

void platform_turn_counterclockwise(){
  hold(general_delay);
  platform_counterclockwise(true);
  hold_arm_back(general_delay);
  fix_cube(general_delay);
  move_arm_back(general_delay);
}

// 1: Red
// 2: Green
// 3: Blue
// 4: White
// 5: Yellow
// 6: Orange

class Rubik{
  public:
    Rubik(){
      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          front[i][j] = 2;
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          up[i][j] = 4;
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          down[i][j] = 5;
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          left[i][j] = 6;
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          right[i][j] = 1;
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          back[i][j] = 3;
        }
      }
    }

    void print_cube(){
      Serial.print("        ");
      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          Serial.print(up[i][j]);
          Serial.print(" ");
        }
        Serial.println();
        if(i <= 1){
          Serial.print("        ");
        }
      }

      Serial.println();

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          Serial.print(left[i][j]);
          Serial.print(" ");
        }
        Serial.print("  ");
        for(int j = 0; j < 3; j++){
          Serial.print(front[i][j]);
          Serial.print(" ");
        }
        Serial.print("  ");
        for(int j = 0; j < 3; j++){
          Serial.print(right[i][j]);
          Serial.print(" ");
        }
        Serial.print("  ");
        for(int j = 0; j < 3; j++){
          Serial.print(back[i][j]);
          Serial.print(" ");
        }
        Serial.println();
      }

      Serial.println();

      Serial.print("        ");
      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          Serial.print(down[i][j]);
          Serial.print(" ");
        }
        Serial.println();
        if(i <= 1){
          Serial.print("        ");
        }
      }

      Serial.println();
    }

    void flip_cube(){
      int temp[N][N];

      for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
          temp[i][j] = front[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          front[i][j] = down[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          down[i][j] = back[i][j];
        }
      }
      rotate_array_clockwise(down);
      rotate_array_clockwise(down);

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          back[i][j] = up[i][j];
        }
      }
      rotate_array_clockwise(back);
      rotate_array_clockwise(back);

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          up[i][j] = temp[i][j];
        }
      }

      rotate_array_clockwise(right);

      rotate_array_counterclockwise(left);

      flip();
    }

    void rotate_clockwise(){
      int temp[N][N];

      for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
          temp[i][j] = front[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          front[i][j] = right[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          right[i][j] = back[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          back[i][j] = left[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          left[i][j] = temp[i][j];
        }
      }

      rotate_array_clockwise(up);

      rotate_array_counterclockwise(down);

      platform_clockwise(false);
    }

    void rotate_counterclockwise(){
      int temp[N][N];

      for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
          temp[i][j] = front[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          front[i][j] = left[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          left[i][j] = back[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          back[i][j] = right[i][j];
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          right[i][j] = temp[i][j];
        }
      }

      rotate_array_counterclockwise(up);

      rotate_array_clockwise(down);

      platform_counterclockwise(false);
    }

    void turn_clockwise(){
      int temp[N];

      for(int i = 0; i < N; i++){
        temp[i] = front[2][i];
      }

      for(int i = 0; i < N; i++){
        front[2][i] = right[2][i];
      }

      for(int i = 0; i < N; i++){
        right[2][i] = back[2][i];
      }

      for(int i = 0; i < N; i++){
        back[2][i] = left[2][i];
      }

      for(int i = 0; i < N; i++){
        left[2][i] = temp[i];
      }

      rotate_array_counterclockwise(down);

      platform_turn_clockwise();
    }

    void turn_counterclockwise(){
      int temp[N];

      for(int i = 0; i < N; i++){
        temp[i] = front[2][i];
      }

      for(int i = 0; i < N; i++){
        front[2][i] = left[2][i];
      }

      for(int i = 0; i < N; i++){
        left[2][i] = back[2][i];
      }

      for(int i = 0; i < N; i++){
        back[2][i] = right[2][i];
      }

      for(int i = 0; i < N; i++){
        right[2][i] = temp[i];
      }

      rotate_array_clockwise(down);

      platform_turn_counterclockwise();
    }

    void algorithm(String algo){
      for(int i = 0; i < algo.length(); i++){
        switch(algo[i]){
          // F
          case 'F':
            flip_cube();
            flip_cube();
            flip_cube();
            turn_counterclockwise();
            flip_cube();
            break;
            
          // F'
          case 'f':
            flip_cube();
            flip_cube();
            flip_cube();
            turn_clockwise();
            flip_cube();
            break;
            
          // D
          case 'D':
            turn_counterclockwise();
            break;
            
          // D'
          case 'd':
            turn_clockwise();
            break;

          // U
          case 'U':
            flip_cube();
            flip_cube();
            turn_counterclockwise();
            flip_cube();
            flip_cube();
            break;

          // U'
          case 'u':
            flip_cube();
            flip_cube();
            turn_clockwise();
            flip_cube();
            flip_cube();
            break;

          // L
          case 'L':
            rotate_clockwise();
            flip_cube();
            turn_counterclockwise();
            flip_cube();
            flip_cube();
            flip_cube();
            rotate_counterclockwise();
            break;

          // L'
          case 'l':
            rotate_clockwise();
            flip_cube();
            turn_clockwise();
            flip_cube();
            flip_cube();
            flip_cube();
            rotate_counterclockwise();
            break;

          // R
          case 'R':
            rotate_counterclockwise();
            flip_cube();
            turn_counterclockwise();
            flip_cube();
            flip_cube();
            flip_cube();
            rotate_clockwise();
            break;

          // R'
          case 'r':
            rotate_counterclockwise();
            flip_cube();
            turn_clockwise();
            flip_cube();
            flip_cube();
            flip_cube();
            rotate_clockwise();
            break;

          // B 
          case 'B':
            flip_cube();
            turn_counterclockwise();
            flip_cube();
            flip_cube();
            flip_cube();
            break;

          // B'
          case 'b':
            flip_cube();
            turn_clockwise();
            flip_cube();
            flip_cube();
            flip_cube();
            break;
            
          case ' ':
            break;
            
          default:
            break;
        }
        if(algo[i] != ' '){
        Serial.print("Move: ");
        Serial.println(algo[i]);
        print_cube();
        delay(3000);
        }
      }
    }

    
    int front[N][N];

    int left[N][N];

    int right[N][N];

    int up[N][N];

    int down[N][N];

    int back[N][N];

  private:

  void rotate_array_clockwise(int (&arr)[N][N]){ 
       // Traverse each cycle 
      for (int i = 0; i < N / 2; i++) { 
        for (int j = i; j < N - i - 1; j++) { 
  
            // Swap elements of each cycle 
            // in clockwise direction 
            int temp = arr[i][j]; 
            arr[i][j] = arr[N - 1 - j][i]; 
            arr[N - 1 - j][i] = arr[N - 1 - i][N - 1 - j]; 
            arr[N - 1 - i][N - 1 - j] = arr[j][N - 1 - i]; 
            arr[j][N - 1 - i] = temp; 
        } 
      } 
    } 

  void rotate_array_counterclockwise(int (&arr)[N][N]) 
  { 
  
       // Traverse each cycle 
      for (int i = 0; i < N / 2; i++) { 
        for (int j = i; j < N - i - 1; j++) { 
  
            // Swap elements of each cycle 
            // in counter clockwise direction 
            int temp = arr[i][j]; 
            arr[i][j] = arr[j][N - 1 - i];
            arr[j][N - 1 - i] = arr[N - 1 - i][N - 1 - j];
            arr[N - 1 - i][N - 1 - j] = arr[N - 1 - j][i];
            arr[N - 1 - j][i] = temp;
      } 
    } 
  } 
    
};

// Define number of steps per rotation:
//const int stepsPerRevolution = 2038;
// Wiring:
// Pin 8 to IN1 on the ULN2003 driver
// Pin 9 to IN2 on the ULN2003 driver
// Pin 10 to IN3 on the ULN2003 driver
// Pin 11 to IN4 on the ULN2003 driver
// Create stepper object called 'myStepper', note the pin order:
//Stepper platform = Stepper(stepsPerRevolution, 8, 10, 9, 11);


void initialize_servos(){
  platform.write(91);
  platform.attach(3, 500, 2700);

  delay(300);

  move_arm.write(172);
  move_arm.attach(6);

  delay(300);

  light_arm.write(180);
  light_arm.attach(9);

  delay(300);

  hold_arm.write(155);
  hold_arm.attach(11);

  delay(300);
}

void setup() {
  // Begin Serial communication at a baud rate of 9600:
  Serial.begin(9600);

  initialize_servos();

  platform_middle(1000);
  
  // Delay to make sure I can always reset program
  delay(2000);
}

int determine_color(double red, double green, double blue){
  // Red
  if(red > green && red > blue && blue > green){
    return 1;
  }
  // Green
  else if(green > blue && green > red && blue > red){
    return 2;
  }
  // Blue
  else if(blue > green && blue > red && green > red){
    return 3;
  }
  // White
  else if(green >= blue && green >= red && abs(blue - red) <= 3000){
    return 4;
  }
  // Yellow
  else if(red >= blue && green > blue && abs(red - green) <= 3000){
    return 5;
  }
  // Orange
  else if(red > green && red > blue && green > blue){
    return 6;
  }
  else{
    return -1;
  }
}

String number_to_color(int num){
  switch(num){
    case 1:
      return "Red";
      break;
    case 2:
      return "Green";
      break;
    case 3: 
      return "Blue";
      break;
    case 4:
      return "White";
      break;
    case 5:
      return "Yellow";
      break;
    case 6:
      return "Orange";
      break;
    default:
      return "Error";
  }
}

Rubik rubik;

void loop() {
  rubik.print_cube();
  //rubik.algorithm("r d R D"); 
  while(1){
    rubik.algorithm("DDUURRLLFFBB");
  }

  Serial.println("DONE");
  delay(2000);
  hold_arm_back(1000);
  move_arm_back(1000);
  delay(2000);
  while(1){
    
    platform_start(500);
    platform_end(500);
  }
}
