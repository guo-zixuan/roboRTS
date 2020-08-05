#ifndef ROBORTS_DECISION_TEST_BEHAVIOR_H
#define ROBORTS_DECISION_TEST_BEHAVIOR_H

#include "io/io.h"

#include "../blackboard/blackboard.h"
#include "../executor/chassis_executor.h"
#include "../behavior_tree/behavior_state.h"
#include "../proto/decision.pb.h"

#include "line_iterator.h"

namespace roborts_decision {

class TestBehavior {
 public:
  TestBehavior(ChassisExecutor* &chassis_executor,
				 Blackboard* &blackboard) : chassis_executor_(chassis_executor),
														blackboard_(blackboard) {

	patrol_count_ = 0;
	point_size_ = 0;

  }

  void Run() {

	auto executor_state = Update();

	std::cout << "state: " << (int)(executor_state) << std::endl;

	if (executor_state != BehaviorState::RUNNING) {

	  geometry_msgs::Twist cmd;
	  cmd.linear.x = 0.2;
	  cmd.linear.y = 0;
	  cmd.linear.z = 0;

	  cmd.angular.x = 0;
	  cmd.angular.y = 0;
	  cmd.angular.z = 0.5;

	  chassis_executor_->Execute(cmd);

	}
  }

  void Cancel() {
	chassis_executor_->Cancel();
  }

  BehaviorState Update() {
	return chassis_executor_->Update();
  }

  ~TestBehavior() = default;

 private:
  //! executor
  ChassisExecutor* const chassis_executor_;

  //! perception information
  Blackboard* const blackboard_;

  //! patrol buffer
  std::vector<geometry_msgs::PoseStamped> patrol_goals_;
  unsigned int patrol_count_;
  unsigned int point_size_;

};
}

#endif //ROBORTS_DECISION_TEST_BEHAVIOR_H
