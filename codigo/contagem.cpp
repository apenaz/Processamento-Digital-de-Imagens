#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

int main(int argc, char** argv){
  Mat image,image2;
  int width, height,nobjects,buraco;
  nobjects=0;
  buraco=0;
CvPoint p;
p.x=0;
p.y=0;

image = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);

if(!image.data){
    std::cout << "imagem nao carregou corretamente\n";
    return(-1);
  }

// Retiramos as bolhas das bordas

width=image.size().width;
 height=image.size().height;

 for (int j=0;j<width;j++){
    image.at<uchar>(0,j)=255;
   image.at<uchar>(height-1,j)=255;
  }
  for (int i=0;i<height;i++){
    image.at<uchar>(i,0)=255;
    image.at<uchar>(i,width-1)=255;
  }

 floodFill(image,p,0);

// Mudaremos o fundo para cinza 
floodFill(image,p,50);

// Contamos e dizemos quantas bolhas tem com buracos  
  for(int i=0; i<height; i++){
    for(int j=0; j<width; j++){
      if(image.at<uchar>(i,j) == 0){
	//// achou um objeto
		buraco++;
		p.x=j;
		p.y=i;
		floodFill(image,p,buraco);
	  }
	}
  }

 // Colorimos todas as bolhas de preto (e assim "acabamos" com os buracos)
 for(int i=0; i < height; ++i){
    for(int j=0; j < width; ++j){
      if(image.at<uchar>(i, j) == 255){
        p.x=j;
        p.y=i;
        floodFill(image, p, 0);
      }
    }
  }
// Fazemos a contagem de quantas bolhas possuem na cena, independente se no inicio possuiam ou não buracos

for(int i=0; i<height; i++){
  for(int j=0; j<width; j++){
    if(image.at<uchar>(i,j) == 0){
      nobjects=nobjects+1;
      p.x=j;
      p.y=i;
      floodFill(image,p,nobjects);
    }
  }
}


std:: cout<<nobjects<<std::endl;
std:: cout<<buraco<<std::endl;
image.copyTo(image2);

imshow("image", image2);
 waitKey();
  return 0;
}
