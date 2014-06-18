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
  //  ros::Publisher image_pub;
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
    //    nh_.param("image_topic", image_topic_, sensor_msgs::Image("/prosilica/image_raw"));
    image_sub_= nh_.subscribe("/prosilica/image_raw", 1, &create_image::Imagecallback, this);
    //    image_pub = nh_.advertise<sensor_msgs::Image>("chatter", 1000);
    //parameters
    bl=false;
    namedWindow("image1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    namedWindow("image2", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    namedWindow("result1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
    namedWindow("result2", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  }
  ~create_image(){
    destroyWindow("image1");
    destroyWindow("image2");
    destroyWindow("result1");
    destroyWindow("result2");
  }
  void Imagecallback(const sensor_msgs::/*Image&*/ImageConstPtr& image )
  {
    ROS_INFO("prosilica");
    publish(image);
  }
  void publish (const sensor_msgs::ImageConstPtr& image){
    if(bl==false){
      cv_ptr1 = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
      cv_ptr2 = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
    }
    else{
      cv_ptr2 = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
    }
    cv::cvtColor(cv_ptr1->image,gray1,CV_BGR2GRAY);
    cv::cvtColor(cv_ptr2->image,gray2,CV_BGR2GRAY);
    cv::absdiff(gray1,gray2,diff); //絶対値を生成
    cv::threshold(diff,diff2,100,255,CV_THRESH_BINARY); // 二值化處理

    imshow("image1", cv_ptr1->image);
    imshow("image2", cv_ptr2->image);
    imshow("result1", diff);
    imshow("result2",diff2 );
    waitKey(3);
  }
  void boolchange(){
    bl=true;
  }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "talk");
  ros::NodeHandle n("~");
  create_image c1(n);
  string a;
  cin>>a;
  if(a=="c"){
    ros::spinOnce();
    c1.boolchange();
  }
  ros::spin();
}


