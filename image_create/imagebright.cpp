#include <ros/ros.h>
#include <ros/node_handle.h>
#include <cstdlib>
#include <string.h>
#include "cv_bridge/cv_bridge.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <boost/thread.hpp>
#include <sensor_msgs/Image.h>
#include <message_filters/subscriber.h>
#include <sensor_msgs/image_encodings.h>
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
using namespace cv_bridge;
class create_image
{
private:
  ros::Subscriber image_sub_;
  ros::NodeHandle nh_;
public:
  string image_topic_;
  bool bl;
  CvImagePtr cv_ptr1, cv_ptr2;
  cv::Mat gray1;
  cv::Mat gray2;
  cv::Mat diff; //差分画像を生成
  cv::Mat diff2;

  create_image (ros::NodeHandle &nh) :nh_ (nh)
  {
    image_sub_= nh_.subscribe("/camera/ir/image_raw", 1, &create_image::Imagecallback, this);
    namedWindow("result", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  }

  ~create_image(){
    destroyWindow("result");
  }

  void Imagecallback(const sensor_msgs::/*Image&*/ImageConstPtr& image )
  {
    publish(image);
  }

  void publish (const sensor_msgs::ImageConstPtr& image){
    cv_ptr1 = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::MONO16);
    cv::Mat img = cv_ptr1->image;
    for (int j = 0; j < img.rows; j++) { // y
      ROS_INFO_STREAM("j: " << j);
      for (int i = 0; i < img.cols; i++) { // x
        ((unsigned short*)img.data)[j * img.cols + i] = ((unsigned short*)img.data)[j * img.cols + i] * 128;
      }
    }
    imshow("result",img);
    waitKey(3);
  }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talk");
  ros::NodeHandle n("~");
  create_image c1(n);
  ros::spin();
}


