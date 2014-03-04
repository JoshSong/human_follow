#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <tf/transform_listener.h>

#define _USE_MATH_DEFINES
#include <math.h>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "talker");
  ros::NodeHandle n;

  ros::Publisher left_wheels_pub = n.advertise<std_msgs::Float64>("left_wheels_speed", 1);
  ros::Publisher right_wheels_pub = n.advertise<std_msgs::Float64>("right_wheels_speed", 1);

  ros::Rate loop_rate(10);
  tf::TransformListener tf_listener;

  while (ros::ok())
  {

  	// Lookup transform of human relative to robot
  	tf::StampedTransform hr_tf;
  	try {
  		tf_listener.lookupTransform("/robot", "/human", ros::Time(0), hr_tf);
  	}
  	catch (tf::TransformException ex) {
  		ROS_ERROR("%s", ex.what());
  	}

  	float dx = hr_tf.getOrigin().x();
  	float dy = hr_tf.getOrigin().y();

  	float dist = sqrt(pow(dx, 2) + pow(dy, 2));
  	float angle = atan2(dy, dx) * 180 / M_PI;

  	//ROS_INFO_STREAM("x: " << dx);
  	//ROS_INFO_STREAM("y: " << dy);
  	//ROS_INFO_STREAM("Angle: " << angle);

  	float l_speed;
  	float r_speed;
  	float max_speed = 5;

  	if (angle >= 0 && angle <= 45) {
  		r_speed = max_speed;
  		l_speed = (1 - angle/45) * max_speed;
  	}
  	else if (angle <= 0 && angle >= -45) {
  		r_speed = (1 + angle/45) * max_speed;
  		l_speed = max_speed;
  	}
  	else if (angle > 45) { // && angle <= 180
  		r_speed = max_speed;
  		l_speed = -max_speed;
  	}
  	else {
  		r_speed = -max_speed;
  		l_speed = max_speed;
  	}

  	//ROS_INFO_STREAM("r_speed: " << r_speed);
  	//ROS_INFO_STREAM("l_speed: " << l_speed);

  	// Publish desired wheel speeds on ROS topic
    std_msgs::Float64 left_speed_msg;
    std_msgs::Float64 right_speed_msg;

    left_speed_msg.data = l_speed;
    right_speed_msg.data = r_speed;

    left_wheels_pub.publish(left_speed_msg);
    right_wheels_pub.publish(right_speed_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
