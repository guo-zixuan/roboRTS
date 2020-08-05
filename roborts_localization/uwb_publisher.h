//
// Created by liyuqi on 2020/8/3.
//

#ifndef SRC_ROBORTS_ROBORTS_LOCALIZATION_UWB_PUBLISHER_H_
#define SRC_ROBORTS_ROBORTS_LOCALIZATION_UWB_PUBLISHER_H_

#include <memory>
#include <ros/ros.h>
#include <serial_device.h>
#include <roborts_msgs/UWB.h>

namespace roborts_localization {

#define GetBit(x, bit) (uint8_t((x >> (bit-1)) & 0x01))

template<typename T>
T Sum16(const uint8_t* buffer){
  T ret = (*(T*)buffer) + 256 * (*(T*)(buffer+1));
  return ret;
}

#pragma pack (1)
struct UWB_frame{
  int16_t GetX(){return Sum16<int16_t>(&(buff[0]));}

  int16_t GetY(){return Sum16<int16_t>(&(buff[2]));}

  uint16_t GetTheta(){return Sum16<uint16_t>(&(buff[4]));}

  int16_t GetDistance(uint8_t idx){
    if(idx >= 6){
      return 0;
    }
    return Sum16<int16_t>(&(buff[6+2*idx]));
  }

  uint8_t GetError(){
	if(buff[18] & 0x01){
	  return 1;
	}
	else if(buff[18] & 0x02){
	  return 2;
	}
	else if(buff[18] & 0x04){
	  return 3;
	}
	else if(buff[18] & 0x08){
	  return 4;
	}
	else return 0;
  }

  uint8_t GetSignal(){
	if(!GetBit(buff[19],7) && !GetBit(buff[19],8)){
	  return 1;
	}
	else if(!GetBit(buff[19],7) && GetBit(buff[19],8)){
	  return 2;
	}
	else if( GetBit(buff[19],7) && !GetBit(buff[19],8)){
	  return 3;
	}
	else if( GetBit(buff[19],7) && GetBit(buff[19],8)){
	  return 4;
	}
	else return 0;
  }

  uint8_t buff[22];
};
#pragma pack ()

class UWB_Publisher {
 public:
  UWB_Publisher() = default;
  ~UWB_Publisher();

  int Init();

  void Run();

  uint16_t GetRunFrequency() const;

 private:

  ros::Publisher publisher_;
  std::unique_ptr<roborts_sdk::SerialDevice> device_ptr_;

  //ROS Node handle
  uint16_t run_frequency;

 private:
  ros::NodeHandle nh_;

};

} // namespace roborts_localization
#endif //SRC_ROBORTS_ROBORTS_LOCALIZATION_UWB_PUBLISHER_H_
