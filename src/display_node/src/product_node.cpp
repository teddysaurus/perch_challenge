#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Int64.h"

#include <sstream>

//void perchProductCallback(const std_msgs::String::ConstPtr& msg);

int main(int argc, char **argv)
{
  ros::init(argc, argv, "perch_product");
  ros::NodeHandle n;
  ros::Publisher product_pub = n.advertise<std_msgs::String>("perch_product", 1000);
  ros::Publisher perch_clock_pub = n.advertise<std_msgs::Int64>("perch_clock", 1);
//  ros::Subscriber        sub = n.subscribe("perch_product", 1000, perchProductCallback);
  ros::Rate loop_rate(60); // Speed up time by 60.
  std_msgs::Int64 fake_clock;
  
  time_t  t = time(0);
  fake_clock.data = t;
  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */

  srand((unsigned) t);

  int count = 0;
  int64_t wait_for_customer = fake_clock.data + rand() % 600 ;
  bool customer_session = false;

  while (ros::ok())
  {
    std_msgs::String msg;
    int product_choice;
    std::string product;
    
    if (fake_clock.data > wait_for_customer)
    {
        std::stringstream ss;
        ss << "PRODUCT";
        if(customer_session)
        {
          ss << product_choice << "/down";
          wait_for_customer = fake_clock.data + rand() % 600 ;
          customer_session = false;
        }
        else
        {
          product_choice = 1 + rand() % 5;
          ss << product_choice << "/up";
          wait_for_customer = fake_clock.data + rand() % 180 ;
          customer_session = true;
        }

        msg.data = ss.str();

        ROS_INFO("%s", msg.data.c_str());
        product_pub.publish(msg);
    }
  
    fake_clock.data++;
    perch_clock_pub.publish(fake_clock);
    

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}

/*void perchProductCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data.c_str());
}*/
