#include <Servo.h>
#include "Adafruit_TCS34725.h"

//   u
// l f r b
//   d
 
// Size of rubik cube
const int N = 3;

const int RED = 1;
const int GREEN = 2;
const int BLUE = 3;
const int WHITE = 4;
const int YELLOW = 5;
const int ORANGE = 6;

Servo light_arm;

Servo move_arm;

Servo hold_arm;

Servo platform;

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

int platform_rotation = 0;

// Delays
const int general_delay = 700;

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
  else if(green >= blue && green >= red && abs(blue - red) <= 5000){
    return 4;
  }
  // Yellow
  else if(red >= blue && green > blue && abs(red - green) <= 5000){
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

void flip(){
  move_arm_back(700);
  
  // Orignally was 95
  move_arm.write(80);
  delay(1000);
  move_arm.write(150);

  /*for(int i = 91; i <= 150; i++){
    move_arm.write(i);
    delay(15);
  }*/
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

// 1: Red
// 2: Green
// 3: Blue
// 4: White
// 5: Yellow
// 6: Orange

    bool check_green_cross(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      // Check top middle piece
      if(up[0][1] != GREEN || back[0][1] != back_center){
        return false;
      }

      // Check right middle piece
      if(up[1][2] != GREEN || right[0][1] != right_center){
        return false;
      }
      
      // Check bottom middle piece
      if(up[2][1] != GREEN || front[0][1] != front_center){
        return false;
      }
      
      // Check left middle piece
      if(up[1][0] != GREEN || left[0][1] != left_center){
        return false;
      }

      return true;
    }

    String find_green_cross(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];
      
      // Look for green edges on the down face for quick solve
      if(down[0][1] == GREEN){
        if(front[2][1] == WHITE){
          return "FF";
        }
        if(front[2][1] == ORANGE){
          return "DRR";
        }
        if(front[2][1] == RED){
          return "dLL";
        }
        if(front[2][1] == YELLOW){
          return "DDBB";
        }
      }
      if(down[1][2] == GREEN){
        if(right[2][1] == ORANGE){
          return "RR";
        }
        if(right[2][1] == YELLOW){
          return "DBB";
        }
        if(right[2][1] == WHITE){
          return "dFF";
        }
        if(right[2][1] == RED){
          return "DDLL";
        }
      }
      if(down[2][1] == GREEN){
        if(back[2][1] == YELLOW){
          return "BB";
        }
        if(back[2][1] == RED){
          return "DLL";
        }
        if(back[2][1] == ORANGE){
          return "dRR";
        }
        if(back[2][1] == WHITE){
          return "DDFF";
        }
      }
      if(down[1][0] == GREEN){
        if(left[2][1] == RED){
          return "LL";
        }
        if(left[2][1] == WHITE){
          return "DFF";
        }
        if(left[2][1] == YELLOW){
          return "dBB";
        }
        if(left[2][1] == ORANGE){
          return "DDRR";
        }
      }

      // Solve bottom pieces that are oriented incorrectly
      if(front[2][1] == GREEN){
        if(down[0][1] == WHITE){
          return "fuRU";
        }
        if(down[0][1] == ORANGE){
          return "DruBU";
        }
        if(down[0][1] == YELLOW){
          return "DDbuLU";
        }
        if(down[0][1] == RED){
          return "dluFU";
        }
      }
      if(right[2][1] == GREEN){
        if(down[1][2] == WHITE){
          return "dfuRU";
        }
        if(down[1][2] == ORANGE){
          return "ruBU";
        }
        if(down[1][2] == YELLOW){
          return "DbuLU";
        }
        if(down[1][2] == RED){
          return "DDluFU";
        }
      }
      if(left[2][1] == GREEN){
        if(down[1][0] == WHITE){
          return "DfuRU";
        }
        if(down[1][0] == ORANGE){
          return "DDrUBu";
        }
        if(down[1][0] == YELLOW){
          return "dbuLU";
        }
        if(down[1][0] == RED){
          return "luFU";
        }
      }
      if(back[2][1] == GREEN){
        if(down[2][1] == WHITE){
          return "DDfuRU";
        }
        if(down[2][1] == ORANGE){
          return "drUBu";
        }
        if(down[2][1] == YELLOW){
          return "buLU";
        }
        if(down[2][1] == RED){
          return "DluFU";
        }
      }

      // Middle pieces
    if(right[1][0] == GREEN){
      return "FDf";
    }
    if(left[1][2] == GREEN){
      return "fDF";
    }

    if(front[1][2] == GREEN){
      return "rDR";
    }
    if(back[1][0] == GREEN){
      return "RDr";
    }

    if(left[1][0] == GREEN){
      return "BDb";
    }
    if(right[1][2] == GREEN){
      return "bDB";
    }

    if(front[1][0] == GREEN){
      return "LDl";
    }
    if(back[1][2] == GREEN){
      return "lDL";
    }

    // Top Pieces
    if(up[2][1] == GREEN || front[0][1] == GREEN){
      if(up[2][1] != GREEN && front[0][1] != front_center){
        return "FF";
      }
    }
    if(up[1][2] == GREEN || right[0][1] == GREEN){
      if(up[1][2] != GREEN && right[0][1] != right_center){
        return "RR";
      }
    }
    if(up[0][1] == GREEN || back[0][1] == GREEN){
      if(up[0][1] != GREEN && back[0][1] != back_center){
        return "BB";
      }
    }
    if(up[1][0] == GREEN || left[0][1] == GREEN){
      if(up[1][0] != GREEN && left[0][1] != left_center){
        return "LL";
      }
    }
    
      return "";
    }

    void solve_green_cross(){
      while(!check_green_cross()){
        String algo = find_green_cross();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    bool check_green_corners(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      // Check if green corners are present
      if(up[0][0] != GREEN || up[0][2] != GREEN || up[2][0] != GREEN || up[2][2] != GREEN){
        return false;
      }

      // Bottom Right Corner
      if(front[0][2] != front_center || right[0][0] != right_center){
        return false;
      }

      // Top Right Corner
      if(right[0][2] != right_center || back[0][0] != back_center){
        return false;
      }

      // Top Left Corner
      if(back[0][2] != back_center || left[0][0] != left_center){
        return false;
      }

      // Bottom Left Corner
      if(left[0][2] != left_center || front[0][0] != front_center){
        return false;
      }

      return true;
    }

    String find_green_corner(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];
      
      if(front[2][2] == GREEN){
        if(down[0][2] == front_center){
          return "FDf";
        }
        if(down[0][2] == right_center){
          return "DRDr";
        }
        if(down[0][2] == left_center){
          return "dLDl";
        }
        if(down[0][2] == back_center){
          return "DDBDb";
        }
      }
      if(right[2][0] == GREEN){
        if(down[0][2] == front_center){
          return "dfdF";
        }
        if(down[0][2] == right_center){
          return "rdR";
        }
        if(down[0][2] == left_center){
          return "DDldL";
        }
        if(down[0][2] == back_center){
          return "DbdB";
        }
      }
      if(down[0][2] == GREEN){
        if(front[2][2] == front_center){
          return "dfddFDfdF";
        }
        if(front[2][2] == right_center){
          return "rddRDrdR";
        }
        if(front[2][2] == left_center){
          return "DDlddLDldL";
        }
        if(front[2][2] == back_center){
          return "DbddBDbdB";
        }
      }
      return "";
    }

    String find_green_corners(){
      if(find_green_corner() != ""){
        return find_green_corner();
      }
      rotate_clockwise();
      if(find_green_corner() != ""){
        return find_green_corner();
      }
      rotate_clockwise();
      if(find_green_corner() != ""){
        return find_green_corner();
      }
      rotate_clockwise();
      if(find_green_corner() != ""){
        return find_green_corner();
      }

      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      if(up[2][2] == GREEN || front[0][2] == GREEN || right[0][0] == GREEN){
        return "rDR";
      }
      if(up[0][2] == GREEN || back[0][0] == GREEN || right[0][2] == GREEN){
        return "bdB";
      }
      if(up[0][0] == GREEN || back[0][2] == GREEN || left[0][0] == GREEN){
        return "Bdb";
      }
      if(up[2][0] == GREEN || front[0][0] == GREEN || left[0][2] == GREEN){
        return "LDl";
      }
      
      return "";
    }

    void solve_green_corners(){
      while(!check_green_corners()){
        String algo = find_green_corners();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    bool check_second_layer(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      if(front[1][0] != front_center || left[1][2] != left_center){
        return false;
      }
      if(front[1][2] != front_center || right[1][0] != right_center){
        return false;
      }
      if(back[1][2] != back_center || left[1][0] != left_center){
        return false;
      }
      if(back[1][0] != back_center || right[1][2] != right_center){
        return false;
      }

      return true;
    }
    
    String find_second_layer(){
      if(front[2][1] != BLUE && down[0][1] != BLUE){
        if(front[2][1] == ORANGE && down[0][1] == WHITE){
          return "DDFdfdrDR";
        }
        if(front[2][1] == ORANGE && down[0][1] == YELLOW){
          return "bDBDRdr";
        }
        if(front[2][1] == WHITE && down[0][1] == ORANGE){
          return "drDRDFdf";
        }
        if(front[2][1] == WHITE && down[0][1] == RED){
          return "DLdldfDF";
        }
        if(front[2][1] == RED && down[0][1] == WHITE){
          return "ddfDFDLdl";
        }
        if(front[2][1] == RED && down[0][1] == YELLOW){
          return "BdbdlDL";
        }
        if(front[2][1] == YELLOW && down[0][1] == ORANGE){
          return "dRdrdbDB";
        }
        if(front[2][1] == YELLOW && down[0][1] == RED){
          return "DlDLDBdb";
        }
      }
      
      return "";
    }

    String find_second_layers(){
      if(find_second_layer() != ""){
        return find_second_layer();
      }
      turn_clockwise();
      if(find_second_layer() != ""){
        return find_second_layer();
      }
      turn_clockwise();
      if(find_second_layer() != ""){
        return find_second_layer();
      }
      turn_clockwise();
      if(find_second_layer() != ""){
        return find_second_layer();
      }

      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      if(front[1][0] != BLUE && left[1][2] != BLUE){
        if(front[1][0] != front_center || left[1][2] != left_center){
          return "LdldfDF";
        }
      }
      if(front[1][2] != BLUE && right[1][0] != BLUE){
        if(front[1][2] != front_center || right[1][0] != right_center){
          return "rDRDFdf";
        }
      }
      if(back[1][2] != BLUE && left[1][0] != BLUE){
        if(back[1][2] != back_center || left[1][0] != left_center){
          return "BdbdlDL";
        }
      }
      if(back[1][0] != BLUE && right[1][2] != BLUE){
        if(back[1][0] != back_center || right[1][2] != right_center){
          return "bDBDRdr";
        }
      }
      
      return "";
    }

    void solve_second_layer(){
      while(!check_second_layer()){
        String algo = find_second_layers();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    bool check_blue_cross(){
      if(down[0][1] == BLUE && down[2][1] == BLUE && down[1][0] == BLUE && down[1][1] == BLUE && down[1][2] == BLUE){
        return true;
      }
      return false;
    }

    String find_blue_cross(){
      if(down[0][1] == BLUE && down[1][2] == BLUE){
        return "DFLDldLDldf";
      }
      if(down[1][2] == BLUE && down[2][1] == BLUE){
        return "FLDldLDldf";
      }
      if(down[2][1] == BLUE && down[1][0] == BLUE){
        return "dFLDldLDldf";
      }
      if(down[1][0] == BLUE && down[0][1] == BLUE){
        return "DDFLDldLDldf";
      }
      if(down[1][0] == BLUE && down[1][2] == BLUE){
        return "FLDldf";
      }
      if(down[0][1] == BLUE && down[2][1] == BLUE){
        return "DFLDldf";
      }
      
      return "FLDldf";
    }

    void solve_blue_cross(){
      while(!check_blue_cross()){
        String algo = find_blue_cross();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    bool check_blue_edge(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      if(front[2][1] == front_center && right[2][1] == right_center && left[2][1] == left_center && back[2][1] == back_center){
        return true;
      }
      return false;
    }

    String find_blue_edge(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      if(front[2][1] == front_center && right[2][1] == right_center){
        return "RDrDRDDrD";
      }
      if(back[2][1] == back_center && right[2][1] == right_center){
        return "BDbDBDDbD";
      }
      if(left[2][1] == left_center && back[2][1] == back_center){
        return "LDlDLDDlD";
      }
      if(front[2][1] == front_center && left[2][1] == left_center){
        return "FDfDFDDfD";
      }
      if(front[2][1] == front_center && back[2][1] == back_center){
        return "LDlDLDDl";
      }
      if(left[2][1] == left_center && right[2][1] == right_center){
        return "BDbDBDDb";
      }

      return "";
    }

    String find_blue_edges(){
      if(find_blue_edge() != ""){
        return find_blue_edge();
      }
      turn_clockwise();
      if(find_blue_edge() != ""){
        return find_blue_edge();
      }
      turn_clockwise();
      if(find_blue_edge() != ""){
        return find_blue_edge();
      }
      turn_clockwise();
      if(find_blue_edge() != ""){
        return find_blue_edge();
      }

      return "BDbDBDDbD";
    }

    void solve_blue_edge(){
      while(!check_blue_edge()){
        String algo = find_blue_edges();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    bool check_blue_corner(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];

      if(front[2][2] != front_center && front[2][2] != right_center && front[2][2] != BLUE){
        return false;
      }
      if(right[2][0] != front_center && right[2][0] != right_center && right[2][0] != BLUE){
        return false;
      }
      if(down[0][2] != front_center && down[0][2] != right_center && down[0][2] != BLUE){
        return false;
      }
      
      if(front[2][0] != front_center && front[2][0] != left_center && front[2][0] != BLUE){
        return false;
      }
      if(left[2][2] != front_center && left[2][2] != left_center && left[2][2] != BLUE){
        return false;
      }
      if(down[0][0] != front_center && down[0][0] != left_center && down[0][0] != BLUE){
        return false;
      }
      
      if(back[2][0] != back_center && back[2][0] != right_center && back[2][0] != BLUE){
        return false;
      }
      if(right[2][2] != back_center && right[2][2] != right_center && right[2][2] != BLUE){
        return false;
      }
      if(down[2][2] != back_center && down[2][2] != right_center && down[2][2] != BLUE){
        return false;
      }
      
      if(back[2][2] != back_center && back[2][2] != left_center && back[2][2] != BLUE){
        return false;
      }
      if(left[2][0] != back_center && left[2][0] != left_center && left[2][0] != BLUE){
        return false;
      }
      if(down[2][0] != back_center && down[2][0] != left_center && down[2][0] != BLUE){
        return false;
      }

      return true;
    }

    String find_blue_corner(){
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];
      
      if(front[2][2] == front_center || front[2][2] == right_center || front[2][2] == BLUE){
        if(right[2][0] == front_center || right[2][0] == right_center || right[2][0] == BLUE){
          if(down[0][2] == front_center || down[0][2] == right_center || down[0][2] == BLUE){
            return "drDLdRDl";
          }      
        }      
      }

      if(front[2][0] == front_center || front[2][0] == left_center || front[2][0] == BLUE){
        if(left[2][2] == front_center || left[2][2] == left_center || left[2][2] == BLUE){
          if(down[0][0] == front_center || down[0][0] == left_center || down[0][0] == BLUE){
            return "DLdrDldR";
          }
        }      
      }
      
      if(back[2][0] == back_center && back[2][0] == right_center || back[2][0] == BLUE){
        if(right[2][2] == back_center && right[2][2] == right_center || right[2][2] == BLUE){
          if(down[2][2] == back_center && down[2][2] == right_center || down[2][2] == BLUE){
            return "DRdlDrdL";
          }
        }      
      }
      
      if(back[2][2] == back_center || back[2][2] == left_center || back[2][2] == BLUE){
        if(left[2][0] == back_center || left[2][0] == left_center || left[2][0] == BLUE){
          if(down[2][0] == back_center || down[2][0] == left_center || down[2][0] == BLUE){
            return "dlDRdLDr";
          }
        }      
      }
      
      return "drDLdRDl";
    }

    void solve_blue_corner(){
      while(!check_blue_corner()){
        String algo = find_blue_corner();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    bool check_solve_cube(){
      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          if(front[i][j] != front[1][1]){
            return false;
          }
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          if(back[i][j] != back[1][1]){
            return false;
          }
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          if(left[i][j] != left[1][1]){
            return false;
          }
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          if(right[i][j] != right[1][1]){
            return false;
          }
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          if(up[i][j] != up[1][1]){
            return false;
          }
        }
      }

      for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
          if(down[i][j] != down[1][1]){
            return false;
          }
        }
      }

      return true;
    }

    String find_solve_cube(){
      if(up[2][2] == BLUE){
        return "U";
      }
      return "rdRD";
    }
    
    void solve_cube(){
      while(!check_solve_cube()){
        String algo = find_solve_cube();
        Serial.print("Algorithm: "); Serial.println(algo);
        algorithm(algo);
      }
    }

    void solve(){
      Serial.println("Starting Solve");
      Serial.println("Solving Green Cross");
      solve_green_cross();
      Serial.println("Solving Green Corners");
      solve_green_corners();
      Serial.println("Orienting for Second Layer Solve");
      orient_cube(GREEN, WHITE);
      Serial.println("Solving Second Layer");
      solve_second_layer();
      Serial.println("Orienting for Blue Cross Solve");
      orient_cube(GREEN, WHITE);
      Serial.println("Solving Blue Cross");
      solve_blue_cross();
      Serial.println("Orienting for Blue Edge Solve");
      orient_cube(GREEN, WHITE);
      Serial.println("Solving Blue Edge");
      solve_blue_edge();
      Serial.println("Orienting for Blue Corner Solve");
      orient_cube(GREEN, WHITE);
      Serial.println("Solving Blue Corner");
      solve_blue_corner();
      Serial.println("Orienting for Final Solve");
      orient_cube(BLUE, WHITE);
      Serial.println("Solving Cube");
      solve_cube();
      Serial.println("Finished Solve");
      hold_arm_back(1000);
      move_arm_back(1000);
      delay(2000);
      while(1){
        platform_start(500);
        platform_end(500);
      }
    }

    int scan(){
      uint16_t r, g, b, c;
      
      double red = 0;
      double green = 0;
      double blue = 0;
      
      int counter = 1;

      for(int i = 0; i < 3; i++){
        tcs.getRawData(&r, &g, &b, &c);        
        Serial.print("Counter: "); Serial.print(counter); Serial.print("Red: "); Serial.print(r); Serial.print("Green: "); Serial.print(g); Serial.print("Blue: "); Serial.print(b); Serial.println();

        if(i > 1){
          red += r;
          green += g;
          blue += b;
        }
        counter++;
      }

      Serial.println(red);
      Serial.println(green);
      Serial.println(blue);
      
      return determine_color(red, green, blue);
    }

    void scan_face(){
      int color = -1;
      
      // Ready platform and light arm
      platform.write(12);
      light_arm.write(180);
      delay(1000);

      // First cube
      Serial.println("First Cube");
      light_arm.write(111);
      color = scan();
      Serial.println(number_to_color(color));
      up[1][2] = color;

      // Second cube
      Serial.println("Second Cube");
      light_arm.write(104);
      color = scan();
      Serial.println(number_to_color(color));
      up[1][1] = color;

      // Third cube
      Serial.println("Third Cube");
      light_arm.write(95);
      color = scan();
      Serial.println(number_to_color(color));
      up[1][0] = color;

      // Platform 45
      platform.write(51);
      delay(1000);
      
      // Fourth cube
      Serial.println("Fourth Cube");
      light_arm.write(116);
      color = scan();
      Serial.println(number_to_color(color));
      up[2][2] = color;

      // Fifth cube
      Serial.println("Fifth Cube");
      light_arm.write(94);
      color = scan();
      Serial.println(number_to_color(color));
      up[0][0] = color;

      // Platform 90
      platform.write(91);
      delay(1000);
      
      // Sixth cube
      Serial.println("Sixth Cube");
      light_arm.write(113);
      color = scan();
      Serial.println(number_to_color(color));
      up[2][1] = color;

      // Seventh cube
      Serial.println("Seventh Cube");
      light_arm.write(97);
      color = scan();
      Serial.println(number_to_color(color));
      up[0][1] = color;

      // Platform 135
      platform.write(131);
      delay(1000);

      // Eighth cube
      Serial.println("Eighth Cube");
      light_arm.write(115);
      color = scan();
      Serial.println(number_to_color(color));
      up[2][0] = color;

      // Ninth cube
      Serial.println("Ninth Cube");
      light_arm.write(94);
      color = scan();
      Serial.println(number_to_color(color));
      up[0][2] = color;

      // Reset platform and light arm
      platform.write(12);
      light_arm.write(180);
      delay(1000);

      print_cube();
    }

    void scan_cube(){
      Serial.println("Start Scanning Cube");
      Serial.println("First Face");
      scan_face();
      flip_cube();
      Serial.println("Second Face");
      scan_face();
      flip_cube();
      Serial.println("Third Face");
      scan_face();
      flip_cube();
      Serial.println("Fourth Face");
      scan_face();
      rotate_counterclockwise();
      flip_cube();
      rotate_clockwise();
      Serial.println("Fifth Face");
      scan_face();
      rotate_counterclockwise();
      flip_cube();
      flip_cube();
      rotate_clockwise();
      Serial.println("Sixth Face");
      scan_face();
      Serial.println("Done Scanning Cube");
    }

    void secure_scan_cube(){
      scan_cube();
      
    }

    void orient_cube(int up_color, int front_color){
      Serial.println("Start Orienting Cube");
      int back_center = back[1][1];
      int right_center = right[1][1];
      int front_center = front[1][1];
      int left_center = left[1][1];
      int up_center = up[1][1];
      int down_center = down[1][1];

      Serial.println("Part 1");
      print_cube();
      if(back_center == up_color){
        Serial.println("Back Center");
        flip_cube();
        flip_cube();
        flip_cube();
      }
      if(down_center == up_color){
        Serial.println("Down Center");
        flip_cube();
        flip_cube();
      }
      if(front_center == up_color){
        Serial.println("Front Center");
        flip_cube();
      }
      if(left_center == up_color){
        Serial.println("Left Center");
        rotate_counterclockwise();
        flip_cube();
        rotate_clockwise();
      }
      if(right_center == up_color){
        Serial.println("Right Center");
        rotate_clockwise();
        flip_cube();
        rotate_counterclockwise();
      }

      back_center = back[1][1];
      right_center = right[1][1];
      front_center = front[1][1];
      left_center = left[1][1];
      up_center = up[1][1];
      down_center = down[1][1];
      
      Serial.println("Part 2");
      print_cube();
      if(back_center == front_color){
        Serial.println("Back Center");
        rotate_clockwise();
        rotate_clockwise();
      }
      if(left_center == front_color){
        Serial.println("Left Center");
        rotate_counterclockwise();
      }
      if(right_center == front_color){
        Serial.println("Right Center");
        rotate_clockwise();
      }
      Serial.println("Done Orienting Cube");
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

void initialize_light_sensor(){
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void initialize_servos(){
  platform.write(91);
  platform.attach(3, 500, 2700);

  delay(300);

  move_arm.write(150);
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

  // Initialize
  Serial.println("Initializing");
  Serial.println("Initializing Light Sensor");
  initialize_light_sensor();
  Serial.println("Initializing Servos");
  initialize_servos();
  Serial.println("Initialized");
  
  // Delay to make sure I can always reset program
  delay(2000);
}

Rubik rubik;

void loop() {
  rubik.scan_cube();
  rubik.rotate_counterclockwise();
  rubik.orient_cube(GREEN, WHITE);
  rubik.print_cube();
  rubik.solve();
}
