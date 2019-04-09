#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
  Mat image;
  CvPoint start, stop;

  image= imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
  if(!image.data){
    cout << "A imagem não pode ser carregada." << endl;
    return 1;
  }

  namedWindow("Imagem escolhida",WINDOW_AUTOSIZE);
  imshow("Imagem escolhida",image);
  
  for(int i=start.x;i<stop.x;i++){
    for(int j=start.y;j<stop.y;j++){
      image.at<uchar>(i,j)=0;
    }
  }
  
  imshow("janela", image);  
  waitKey();

  image= imread("bolhas.png",CV_LOAD_IMAGE_COLOR);

  
  for(int i=200;i<210;i++){
    for(int j=10;j<200;j++){
      image.at<Vec3b>(i,j)=100;
    }
  }

  imshow("janela", image);  
  waitKey();
  return 0;
}
