#include <opencv2/opencv.hpp>
#include <stdio.h>

int main(int argc,char **argv)
{

  cv::Mat img1=cv::imread("frame0001.jpg",1);
  cv::Mat img2=cv::imread("frame0002.jpg",1);

  if(img1.empty() || img2.empty())
    {
      std::cout << " Can not find the files " << "\n";
      return -1;
    }

  cv::Mat gray1;
  cv::Mat gray2;
  cv::Mat diff; //差分画像を生成
  cv::Mat diff2;
  cv::cvtColor(img1,gray1,CV_BGR2GRAY);
  cv::cvtColor(img2,gray2,CV_BGR2GRAY);

  //windows生成
  cv::namedWindow("image1", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("image",  CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
  cv::namedWindow("matching result", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);

  cv::absdiff(gray1,gray2,diff); //絶対値を生成
  cv::threshold(diff,diff2,100,255,CV_THRESH_BINARY); // 二值化處理

  cv::imshow("image1", img1);
  cv::imshow("image2", img2);
  cv::imshow("diff", diff);
  cv::imshow("matching result",diff2);
  cv::imwrite("diff.jpg", diff);
  cv::imwrite("diff2.jpg", diff2);
  cv::waitKey(0);
  return 0;

}
