
#include <ros/ros.h>
#include <stdio.h>

#include <rpwc_msgs/PCloudImageReq.h>
#include <rpwc_msgs/PointCloudImage.h>
#include <rpwc_msgs/cameraInfo.h>
#include <sensor_msgs/PointCloud2.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/Image.h>
#include <std_srvs/Empty.h>

sensor_msgs::PointCloud2 point_cloud_;
sensor_msgs::Image image_;
sensor_msgs::CameraInfo cam_info_;

bool callbackServerCamera(rpwc_msgs::PCloudImageReq::Request  &req, rpwc_msgs::PCloudImageReq::Response &res)
{
    res.data.pointCloud = point_cloud_;
    res.data.image = image_;
	return true;
}

bool callbackServerCameraInfo(rpwc_msgs::cameraInfo::Request  &req, rpwc_msgs::cameraInfo::Response &res)
{
	res.cameraInfo = cam_info_;
	return true;
}

void callback_image(const sensor_msgs::ImageConstPtr& msg)
{
  image_ = *msg;
}

void callback_point_cloud (const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
{
	point_cloud_ = *cloud_msg;
}

void callback_camera_info (const sensor_msgs::CameraInfoConstPtr& info_msg)
{
	cam_info_ = *info_msg;
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

	ros::ServiceServer serverCamera = nh.advertiseService("cameraRGBD", callbackServerCamera);
	ros::ServiceServer serverCameraInfo = nh.advertiseService("camera/info", callbackServerCameraInfo);

    image_transport::ImageTransport it(nh);
	image_transport::Subscriber sub_image = it.subscribe("camera/color/image_rect_color", 1, &callback_image);
    ros::Subscriber sub_point_cloud = nh.subscribe("camera/depth_registered/points", 1, &callback_point_cloud);
    ros::Subscriber sub_camera_info = nh.subscribe("camera/color/camera_info", 1, &callback_camera_info);
	ros::Publisher PCloudImage = nh.advertise<rpwc_msgs::PointCloudImage>("cameraRGBD", 1);
	
	while (ros::ok())
	{
		rpwc_msgs::PointCloudImage tmp_data;
		tmp_data.pointCloud = point_cloud_;
		tmp_data.image = image_;
		PCloudImage.publish(tmp_data);
		ros::spinOnce();
		rate.sleep();
	}
	return 0;
}
