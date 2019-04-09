#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image, imageGray, imageEqualize;
  int width, height;
  VideoCapture cap;

  Mat histO, histE;
  int nbins = 128;
  float range[] = {0, 256};
  const float *histrange = {range};
  bool uniform = true;
  bool acummulate = false;

  cap.open(0);

  if (!cap.isOpened())
  {
    cout << "cameras indisponiveis";
    return -1;
  }

  width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
  height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

  cout << "largura = " << width << endl;
  cout << "altura  = " << height << endl;

  int histw = nbins, histh = nbins / 2;
  Mat histImgO(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat histImgE(histh, histw, CV_8UC3, Scalar(0, 0, 0));

  while (1)
  {
    cap >> image;
    cvtColor(image, imageGray, CV_BGR2GRAY);
    equalizeHist(imageGray, imageEqualize);

    calcHist(&imageGray, 1, 0, Mat(), histO, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&imageEqualize, 1, 0, Mat(), histE, 1,
             &nbins, &histrange,
             uniform, acummulate);

    normalize(histO, histO, 0, histImgO.rows, NORM_MINMAX, -1, Mat());
    normalize(histE, histE, 0, histImgE.rows, NORM_MINMAX, -1, Mat());

    histImgO.setTo(Scalar(0));
    histImgE.setTo(Scalar(0));

    for (int i = 0; i < nbins; i++)
    {
      line(histImgO,
           Point(i, histh),
           Point(i, histh - cvRound(histO.at<float>(i))),
           Scalar(128), 1, 8, 0);
    }
    for (int i = 0; i < nbins; i++)
    {
      line(histImgE,
           Point(i, histh),
           Point(i, histh - cvRound(histE.at<float>(i))),
           Scalar(128), 1, 8, 0);
    }
    histImgO.copyTo(imageGray(Rect(0, 0, nbins, histh)));
    histImgE.copyTo(imageEqualize(Rect(0, 0, nbins, histh)));

    imshow("image gray", imageGray);
    imshow("image equalize", imageEqualize);
    if (waitKey(30) >= 0)
      break;
  }
  return 0;
}
