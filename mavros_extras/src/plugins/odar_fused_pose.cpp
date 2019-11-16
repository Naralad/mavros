#include <mavros/mavros_plugin.h>
#include <pluginlib/class_list_macros.h>
#include <iostream>
#include <geometry_msgs/PoseStamped.h>

namespace mavplugin {

	class OdarFusedPosePlugin : public MavRosPlugin {
		public:
			OdarFusedPosePlugin() :
				nh("~odar"),
				uas(nullptr)
		{ };

			void initialize(UAS &uas_)
			{
				uas = &uas_;
				odar_fused_pose_sub = nh.subscribe("fused_pose", 1, &OdarFusedPosePlugin::odar_fused_pose_cb, this);
			};

			const message_map get_rx_handlers() {
				return {/* RX disabled */ };
			}

		private:
			ros::NodeHandle nh;
			UAS *uas;
			ros::Subscriber odar_fused_pose_sub;

			void odar_fused_pose_cb(const geometry_msgs::PoseStamped::ConstPtr &req)
			{

				mavlink_message_t msg;
				mavlink_msg_odar_fused_pose_pack_chan(UAS_PACK_CHAN(uas), &msg, 
					req->header.stamp.toNSec() / 1000,
					req->pose.position.x,
					req->pose.position.y,
					req->pose.position.z,
					req->pose.orientation.w,
					req->pose.orientation.x,
					req->pose.orientation.y,
					req->pose.orientation.z);
				UAS_FCU(uas)->send_message(&msg);

		

			}
	};
};

PLUGINLIB_EXPORT_CLASS(mavplugin::OdarFusedPosePlugin, mavplugin::MavRosPlugin)
