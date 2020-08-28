#include <ros/ros.h>
#include <wiringPi.h>

#include <termios.h>

#define LED_PIN 12

int input_return_key()
{
  struct termios org_term;

  char input_key = 0;

  tcgetattr(STDIN_FILENO, &org_term);

  struct termios new_term = org_term;

  new_term.c_lflag &= ~(ECHO | ICANON);

  new_term.c_cc[VMIN] = 0;
  new_term.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_term);

  read(STDIN_FILENO, &input_key, 1);

  tcsetattr(STDIN_FILENO, TCSANOW, &org_term);

  return input_key;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pigeon_gpio_node");
  ros::NodeHandle nh;

  int a = 0;

  ros::Rate loop_rate(1);

  wiringPiSetupGpio();
  pinMode(LED_PIN, OUTPUT);

  while(ros::ok)
  {
    if( a == 0){
      digitalWrite(LED_PIN,HIGH);
      a = 1;
    }
    else if(a==1){
      digitalWrite(LED_PIN,LOW);
      a = 0;
    }
    loop_rate.sleep();
    ros::spinOnce();
    if(input_return_key() == 27){
      digitalWrite(LED_PIN,LOW);
      return 0;
    }
  }
}
