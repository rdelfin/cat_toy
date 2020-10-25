#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <wiringPi.h>

const int pwm_pin = 1;
double pwm_value = 0.0;

/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void chatterCallback(const std_msgs::Float64::ConstPtr& msg) {
  pwm_value = msg->data;
  ROS_INFO("I heard: [%f]", msg->data);
}

int main(int argc, char **argv)
{
  if (wiringPiSetup () == -1)
    exit(1);
  pinMode(pwm_pin, PWM_OUTPUT);

  ros::init(argc, argv, "motor_controller");
  ros::NodeHandle n;
  ros::Rate r(10);
  ros::Subscriber sub = n.subscribe("motor_angle", 1000, chatterCallback);
  while(ros::ok()) {
    if (pwm_value > 1.0)
      pwm_value = 1.0;
    if (pwm_value < 0.0)
      pwm_value = 0.0;


    ROS_INFO("Writing value: [%d]", (int)(pwm_value * 1023.0));
    pwmWrite(pwm_pin, (int)(pwm_value * 1023.0));
    ros::spinOnce();
    r.sleep();
  }

  return 0;
}
