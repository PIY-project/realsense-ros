
#include <ros/ros.h>
#include <stdio.h>

#include <rpwc_msgs/PCloudImageReq.h>
#include <rpwc_msgs/PointCloudImage.h>
#include <rpwc_msgs/cameraInfo.h>
#include <rpwc_msgs/setSettingsCamera.h>
#include <rpwc_msgs/getSettingsCamera.h>
#include <rpwc_msgs/suggestSettingsCamera.h>
#include <sensor_msgs/PointCloud2.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <std_srvs/Empty.h>

sensor_msgs::PointCloud2::ConstPtr point_cloud_ = boost::make_shared<sensor_msgs::PointCloud2>();
sensor_msgs::Image::ConstPtr image_ = boost::make_shared<sensor_msgs::Image>();
sensor_msgs::CameraInfo::ConstPtr cam_info_ = boost::make_shared<sensor_msgs::CameraInfo>();


bool callbackServerCamera(rpwc_msgs::PCloudImageReq::Request  &req, rpwc_msgs::PCloudImageReq::Response &res)
{
    res.data.pointCloud = *point_cloud_;
    res.data.image = *image_;
	return true;
}

bool callbackServerCameraInfo(rpwc_msgs::cameraInfo::Request  &req, rpwc_msgs::cameraInfo::Response &res)
{
	res.cameraInfo = *cam_info_;
	return true;
}

bool callbackServerSetSettings(rpwc_msgs::setSettingsCamera::Request &req, rpwc_msgs::setSettingsCamera::Response &res)
{
	ROS_WARN("Funzione non supportata dalla camera!");
	return true;
}

bool callbackServerGetSettings(rpwc_msgs::getSettingsCamera::Request& req, rpwc_msgs::getSettingsCamera::Response& res)
{
	ROS_WARN("Funzione non supportata dalla camera!");
	return true;
}

bool callbackServerSuggestSettings(rpwc_msgs::suggestSettingsCamera::Request& req, rpwc_msgs::suggestSettingsCamera::Response& res)
{
	ROS_WARN("Funzione non supportata dalla camera!");
	return true;
}

void callback_image(const sensor_msgs::ImageConstPtr& msg)
{
  	image_ = msg;
	return;
}

void callback_point_cloud (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
{
	point_cloud_ = cloud_msg;
	return;
}

void callback_camera_info (const sensor_msgs::CameraInfoConstPtr& info_msg)
{
	cam_info_ = info_msg;
	return;
}

int main(int argc, char** argv)
{
	//----------------------------------------------------------
	// Preparations
	//----------------------------------------------------------

	// Initialize the node.
	ros::init(argc, argv, "realsense_bridge_node");
	ros::NodeHandle nh;
	ros::Rate rate(5);

	ros::ServiceServer serverCamera = nh.advertiseService("sensorMeas", callbackServerCamera);
	ros::ServiceServer serverCameraInfo = nh.advertiseService("camera/info", callbackServerCameraInfo);
	ros::ServiceServer serverSetSettings = nh.advertiseService("set_settings_camera", callbackServerSetSettings);
	ros::ServiceServer serverGetSettings = nh.advertiseService("get_settings_camera", callbackServerGetSettings);
	ros::ServiceServer serverSuggestSettings = nh.advertiseService("suggest_settings_camera", callbackServerSuggestSettings);

    image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub_image = it.subscribe("camera/color/image_rect_color", 1, &callback_image);
    ros::Subscriber sub_point_cloud = nh.subscribe("camera/depth_registered/points", 1, &callback_point_cloud);
    ros::Subscriber sub_camera_info = nh.subscribe("camera/color/camera_info", 1, &callback_camera_info);
	
	ros::spin();

	return 0;
}
