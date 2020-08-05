//
// Created by liyuqi on 2020/8/3.
//

#include <string>
#include "uwb_publisher.h"
#include "localization_config.h"

roborts_localization::UWB_Publisher::~UWB_Publisher() {

}

int roborts_localization::UWB_Publisher::Init() {
  LocalizationConfig localization_config;
  localization_config.GetParam(&nh_);

  if(!localization_config.enable_uwb)
    return -1;

  std::string ubw_device_tag = std::move(localization_config.uwb_device_tag);
  std::string ubw_frame_id = std::move(localization_config.uwb_frame_id);
  std::string ubw_topic = std::move(localization_config.uwb_topic_name);

  // unused parameters
//  bool use_sim_uwb = localization_config.use_sim_uwb;

  run_frequency = localization_config.uwb_correction_frequency;

  device_ptr_ = std::make_unique<roborts_sdk::SerialDevice>(ubw_device_tag,115200);
  device_ptr_->Init();

  publisher_ = nh_.advertise<roborts_msgs::UWB>(ubw_topic,8);
  return 0;

}

void roborts_localization::UWB_Publisher::Run() {

  UWB_frame frame{};
//  uint8_t frame[22];
  device_ptr_->Read((uint8_t*)&frame, 22);

  roborts_msgs::UWB msg;
  msg.header.stamp = ros::Time::now();

  msg.x = frame.GetX();
  msg.y = frame.GetY();
  msg.theta = frame.GetTheta();
  ROS_ERROR("UWB X: %d, Y: %d, Theta: %d, Error %d, Signal %d",
  	msg.x, msg.y, msg.theta, frame.GetError(), frame.GetSignal());
  publisher_.publish(msg);
}


uint16_t roborts_localization::UWB_Publisher::GetRunFrequency() const {
  return run_frequency;
}

