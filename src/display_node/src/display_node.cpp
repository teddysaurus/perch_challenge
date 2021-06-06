#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int64.h"

#include <sstream>

void perchProductCallback(const std_msgs::String::ConstPtr& msg);
void perchClockCallback(const std_msgs::Int64::ConstPtr& msg);
// Simulate Perch Touchscreen

int64_t g_fake_clock;
int g_selected_product = 0;
// Note, review, video etc, have the same index as the array in ros_redis.py

typedef enum states {
  waiting = 0,
  menu,
  review,
  video,
  accessories,
  closed,
} State;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "perch_display");
  ros::NodeHandle n;
  ros::Publisher display_pub = n.advertise<std_msgs::String>("perch_screen", 1);
  ros::Subscriber product_sub = n.subscribe("perch_product", 1, perchProductCallback);
  ros::Subscriber fake_clock_sub = n.subscribe("perch_clock",1, perchClockCallback);
  ros::Rate loop_rate(10);

  
  State state = waiting;
  
  int count = 0;
  while (ros::ok())
  {
    std_msgs::String msg;
    std::stringstream ss;

    switch (state)
    {
    case waiting:
      if(g_selected_product > 0)
        state = menu;
      break;
    case menu:
      /* show the product menu */
      if(g_selected_product == 0)
      {
        state = waiting;
        break;
      }
      if(!(rand() % 5))
      {
        ss << "PRODUCT" << g_selected_product << "/" << "video";
        state = video;
      }
      if(!(rand() % 5))
      {
        ss << "PRODUCT" << g_selected_product << "/" << "review";
        state = review;   
      }
      if(!(rand() % 5))
      {
        ss << "PRODUCT" << g_selected_product << "/" << "accessories";
        state = accessories;   
      }
      break;
    case video:
      /* show the video */
      
      if(!(rand() % 5))
      {
        ss << "PRODUCT" << g_selected_product << "/" << "close";
        state = closed;
      }
      break;
    case review:
      /* show review */
      
      if (!(rand() % 5))
      {
        ss << "PRODUCT" << g_selected_product << "/" << "close";
        state = closed;
      }
     break;
    case accessories:
      /* show accessories */
      if((rand() % 50))
        break; 
      ss << "PRODUCT" << g_selected_product << "/" << "close";
      state = closed;
    case closed:
      state = waiting;
      break;
    default:
      break;
    }

    if(!ss.str().empty())
    {
      msg.data = ss.str();
      ROS_INFO("%s", msg.data.c_str());
      display_pub.publish(msg);
    }
    
    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}

void perchProductCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("[%Ld] I heard: [%s] ", g_fake_clock, msg->data.c_str());
  char product[] = "PRODUCT";
  std::string action = msg->data.substr(msg->data.find("/"));
  ROS_INFO("%s", action.c_str());
  if(action == "/up")
    g_selected_product = std::stoi(msg->data.substr(strlen(product), 1));
  else
    g_selected_product = 0;
  ROS_INFO("[%Ld] I heard: [%s] (%s, %d)", g_fake_clock, msg->data.c_str(), action.c_str(), g_selected_product);
}

void perchClockCallback(const std_msgs::Int64::ConstPtr& msg)
{
    g_fake_clock = msg->data;
}