//
// Created by liyuqi on 2020/8/3.
//

#include "ros/ros.h"

#include "uwb_publisher.h"
#include "log.h"

int main(int argc, char **argv) {
  roborts_localization::GLogWrapper glog_wrapper(argv[0]);
  ros::init(argc, argv, "localization_node");
  roborts_localization::UWB_Publisher uwb_publisher;

  if (uwb_publisher.Init() < 0){
    ROS_WARN("UWB Node disable or failed init");
    return 0;
  }

  ros::Rate loop_rate(uwb_publisher.GetRunFrequency());
  while(ros::ok()){
    uwb_publisher.Run();
    loop_rate.sleep();
  }
  return 0;
}