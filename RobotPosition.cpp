
#include <ros/ros.h>
#include "gazebo_msgs/ModelStates.h"
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3.h>
#define DEG2RAD (M_PI / 180.0)
#define RAD2DEG (180.0 / M_PI)
#define WHEEL_DIA 0.08
struct quarternion
{  double w;
  double x;
  double y;
  double z;
};
quarternion q1;
double gazebo_x = 0;
double gazebo_y = 0;
double initial_pos_x =-0.8;
double initial_pos_y = 0.4;
double cal_x =-0.8;
double cal_y = 0.4;
double pre_w =0;
double angle_k = 0.5;
double angle_d = 0.1;
double vel = 0.4;
class TetrixDrive
{ public:
  TetrixDrive();
  ~TetrixDrive();
  
  bool init();
  bool controlLoop();
 private:
  // ROS NodeHandle
  ros::NodeHandle nh_;
  ros::NodeHandle nh_priv_;
  // ROS Parameters
  // ROS Time
  // ROS Topic Publishers
  
  ros::Publisher cmd_vel_pub_;
  ros::Publisher pub;
  //ros::Publisher position;
  //ros::Publisher robot_msg;
  // ROS Topic Subscribers
  ros::Subscriber odom_sub_;
  ros::Subscriber state_sub;
  ros::Subscriber cmd_turns;
  ros::Subscriber cmd_robot;
  // Variables
  // Function prototypes
  void updatecommandENCODER(const geometry_msgs::Vector3::ConstPtr &msg);
  void pointcallback(const geometry_msgs::Vector3::ConstPtr &msg);
  void callback(const gazebo_msgs::ModelStates::ConstPtr& msg);
};
geometry_msgs::Twist move_func(double x, double y, quarternion quar, double dest_x, double dest_y)
{  quarternion q = quar;
  double angle;
  
  double t;
  double w;
  double angle_error;
  t = 1;
  angle = atan2(2 * (q.w * q.z + q.x * q.y), 1 - (2 * (pow(q.y, 2) + pow(q.z, 2))));
  double dest_angle = atan2(dest_y - y, dest_x - x);
  double dist = sqrt(pow((dest_x - x), 2) + pow((dest_y - y), 2));
   w = abs(dest_angle - angle);
  
  ROS_INFO_STREAM("dest_angle:" << dest_angle);
  ROS_INFO_STREAM("robot_angle:" << angle);
  ROS_INFO_STREAM("error:" << w);
  geometry_msgs::Twist robot_msg;
  if (dist < 0.05)
  {
    robot_msg.linear.x = 0;
    robot_msg.angular.z = 0;
  }
  else if (w>0.35)
  {
    if (angle > dest_angle)
    {
      robot_msg.angular.z = -angle_k *w  + angle_d*(pre_w - w);
    }
    else
    {
      robot_msg.angular.z = angle_k* w -angle_d*(pre_w - w);
    }
  }
  else{
  robot_msg.linear.x = vel*dist;
  }
  pre_w = w;
  return robot_msg;
}TetrixDrive::TetrixDrive()
    : nh_priv_("~")
{  // Init gazebo ros turtlebot3 node
  ROS_INFO("Ungrogong Simulation Node Init");
  auto ret = init();
  ROS_ASSERT(ret);
}TetrixDrive::~TetrixDrive()
{  ros::shutdown();
}/*******************************************************************************
 * Init function
 *******************************************************************************/
bool TetrixDrive::init()
{   // initialize publishers
  cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  
  // initialize subscribers
  state_sub = nh_.subscribe<gazebo_msgs::ModelStates>("/gazebo/model_states",10,&TetrixDrive::callback, this);
  cmd_turns = nh_.subscribe<geometry_msgs::Vector3>("/encoder", 10, &TetrixDrive::updatecommandENCODER, this);
  cmd_robot =  nh_.subscribe<geometry_msgs::Vector3>("/robot_odr_cmd", 10, &TetrixDrive::pointcallback, this);
   // encoder_sub = nh_.subscribe("encoder", 10, &TetrixDrive::)
  return true;
}void TetrixDrive::updatecommandENCODER(const geometry_msgs::Vector3::ConstPtr &msg)
{  // float r = (msg->x * WHEEL_DIA + msg->y * WHEEL_DIA) / 2;
  // float theta = asin((0.08 * (msg->x - msg->y)) / 0.260);
  // cal_x = r * cos(theta) +initial_pos_x;
  // cal_y = r * sin(theta) + initial_pos_y;
  // ROS_INFO_STREAM("encoder x:" << cal_x);
  // ROS_INFO_STREAM("encoder y:" << cal_y);
  if(msg->z == 1)
  {
    cal_x = -1;
    cal_y = 0.01;
    vel = 0.1;
  }
  if(msg->z == 2)
  {
    cal_x = -1;
    cal_y = -0.4;
    vel = 0.1;
  }
     
  
}void TetrixDrive::pointcallback(const geometry_msgs::Vector3::ConstPtr &msg)
{//   double initial_pos_x =-0.8;
// double initial_pos_y = 0.4;
vel = 0.4;
  if(msg->x == 0)
  {
    cal_x = -0.8;
    cal_y = 0.4;
  }
  if(msg->x == 1)
  {
    cal_x = -0.3;
    cal_y = 0.4;
  }
  if(msg->x == 2)
  {
    cal_x = 0.15;
    cal_y = 0.4;
  }
  if(msg->x == 3)
  {
    cal_x = 0.6;
    cal_y = 0.4;
  }
  if(msg->x == 5)
  {
    cal_x = -0.3;
    cal_y = 0;
  }
  if(msg->x == 6)
  {
    cal_x = 0.15;
    cal_y = 0;
  }
  if(msg->x == 7)
  {
    cal_x = 0.6;
    cal_y = 0;
  }
  if(msg->x == 9)
  {
    cal_x = -0.3;
    cal_y = -0.4;
  }
  if(msg->x == 10)
  {
    cal_x = 0.15;
    cal_y = -0.4;
  }
  if(msg->x == 11)
  {
    cal_x = 0.6;
    cal_y = -0.4;
  }  
  
  
}void TetrixDrive::callback(const gazebo_msgs::ModelStates::ConstPtr& msg)
{    //ROS_INFO_STREAM("robot name:" << msg->name[2]);
    ROS_INFO_STREAM("Position:" << msg->pose[2].position.x << "," << msg->pose[2].position.y << "," << msg->pose[2].position.z);
    quarternion robot_quar;
    robot_quar.w = msg->pose[2].orientation.w;
    robot_quar.x = msg->pose[2].orientation.x;
    robot_quar.y = msg->pose[2].orientation.y;
    robot_quar.z = msg->pose[2].orientation.z;
    q1 =robot_quar;
    gazebo_x = msg->pose[2].position.x;
    gazebo_y = msg->pose[2].position.y;
    geometry_msgs::Twist robot_con;
  robot_con = move_func(gazebo_x,gazebo_y, q1, cal_x, cal_y);
  
  cmd_vel_pub_.publish(robot_con);
}/*******************************************************************************
 * Control Loop function
 *******************************************************************************/
bool TetrixDrive::controlLoop()
{  return true;
}/*******************************************************************************
 * Main function
 *******************************************************************************/
int main(int argc, char *argv[])
{  ros::init(argc, argv, "tetrix_drive");
  TetrixDrive tetrix_drive;
  ros::Rate loop_rate(125);
  while (ros::ok())
  {
    tetrix_drive.controlLoop();
    
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
