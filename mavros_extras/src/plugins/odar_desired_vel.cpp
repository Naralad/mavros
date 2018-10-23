#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>
#include <iostream>
#include <geometry_msgs/TwistStamped.h>

namespace mavplugin {

	class OdarDesiredVelPlugin : public MavRosPlugin {
		public:
			OdarDesiredVelPlugin() :
				nh("~odar"),
				uas(nullptr)
		{ };

			void initialize(UAS &uas_)
			{
				uas = &uas_;
				odar_desired_vel_sub = nh.subscribe("desired_vel", 10, &OdarDesiredVelPlugin::odar_desired_vel_cb, this);
			};

			const message_map get_rx_handlers() {
				return {/* RX disabled */ };
			}

		private:
			ros::NodeHandle nh;
			UAS *uas;
			ros::Subscriber odar_desired_vel_sub;

			void odar_desired_vel_cb(const geometry_msgs::TwistStamped::ConstPtr &req)
			{

				mavlink_message_t msg;
				mavlink_msg_odar_desired_vel_pack_chan(UAS_PACK_CHAN(uas), &msg, 
					req->header.stamp.toNSec() / 1000,
					req->twist.linear.x,
					req->twist.linear.y,
					req->twist.linear.z,
					req->twist.angular.x,
					req->twist.angular.y,
					req->twist.angular.z);
				UAS_FCU(uas)->send_message(&msg);

		

			}
	};
};

PLUGINLIB_EXPORT_CLASS(mavplugin::OdarDesiredVelPlugin, mavplugin::MavRosPlugin)
