#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  Mat image;
  int width, height;
  VideoCapture cap;
  vector<Mat> planes;
  Mat histN, histO;
  int nbins = 64;
  double d;
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
  Mat histImgN(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  Mat histImgO(histh, histw, CV_8UC3, Scalar(0, 0, 0));
  // não dar erro na primeira troca de valores
  // histN.copyTo(histO);	histImgN.copyTo(histImgO);
  cap >> image;
  split(image, planes);
  calcHist(&planes[0], 1, 0, Mat(), histN, 1,
           &nbins, &histrange,
           uniform, acummulate);
  while (1)
  {
    cap >> image;
    split(image, planes);

    histN.copyTo(histO);
    histImgN.copyTo(histImgO);

    calcHist(&planes[0], 1, 0, Mat(), histN, 1,
             &nbins, &histrange,
             uniform, acummulate);
    normalize(histN, histN, 0, histImgN.rows, NORM_MINMAX, -1, Mat());

    histImgN.setTo(Scalar(0));

    for (int i = 0; i < nbins; i++)
    {
      line(histImgN,
           Point(i, histh),
           Point(i, histh - cvRound(histN.at<float>(i))),
           Scalar(0, 0, 255), 1, 8, 0);
    }
    histImgN.copyTo(image(Rect(0, 0, nbins, histh)));
    histImgO.copyTo(image(Rect(0, histh, nbins, histh)));

    d = compareHist(histN, histO, CV_COMP_CORREL);
    cout << d << endl;
    if (d < 0.950)
    {
      cout << "Movimento Detectado" << endl;
      imwrite("Detectado Movimento.png", image);
      imshow("Detectado Movimento", image);
    }

    imshow("image", image);
    if (waitKey(30) >= 0)
      break;
  }
  return 0;
}
