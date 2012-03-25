/**
 * @file SURF_detector
 * @brief SURF keypoint detection + keypoint drawing with OpenCV functions
 * @author A. Huaman
 */

#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

void readme();

/**
 * @function main
 * @brief Main function
 */

template <typename T>
std::string ToString(T e)
{
  std::ostringstream os;
	os << e;
	return std::string(os.str());
}

bool ImgMatch(const std::string &fImg, const std::string &sImg, int count)
{
  cv::Mat Img1 = cv::imread(fImg,CV_LOAD_IMAGE_GRAYSCALE);
  cv::Mat Img2 = cv::imread(sImg,CV_LOAD_IMAGE_GRAYSCALE);
  
	if ( !Img1.data || !Img2.data )
	{
	  std::cerr << "open file failed!" << std::endl;
		return false;
	}
  
	cv::SiftFeatureDetector fdet,sdet;
	cv::SiftDescriptorExtractor fExt,sExt;
  
	std::vector<cv::KeyPoint> fvKP,svKP;

	cv::Mat fR;
	cv::Mat sR;
	fdet.detect(Img1,fvKP);
	sdet.detect(Img2,svKP);
   
  fExt.compute(Img1,fvKP,fR);
  sExt.compute(Img2,svKP,sR);

	cv::BruteForceMatcher<cv::L2<float> > matcher;
	std::vector<cv::DMatch> matches;
	matcher.match(fR,sR,matches);

	cv::Mat iMatches;
	cv::drawMatches(Img1,fvKP,Img2,svKP,matches,iMatches,cv::Scalar(255,255,255));
	IplImage pImg = iMatches;
	cvSaveImage(("/home/beyond_boy/www/match_"+ToString(count)+".png").c_str(),&pImg);


  std::cout << "<H1>match over!<h1/>" << std::endl;
	return true;
}

int main( int argc, char** argv )
{
  if( argc != 3 )
  { readme(); return -1; }
	
	ImgMatch(argv[1],argv[2],1);
  /*Mat img_1 = imread( argv[1], CV_LOAD_IMAGE_GRAYSCALE );
  Mat img_2 = imread( argv[2], CV_LOAD_IMAGE_GRAYSCALE );
  
  if( !img_1.data || !img_2.data )
  { std::cout<< " --(!) Error reading images " << std::endl; return -1; }

  //-- Step 1: Detect the keypoints using SURF Detector
  int minHessian = 400;

  SurfFeatureDetector detector( minHessian );

  std::vector<KeyPoint> keypoints_1, keypoints_2;

  detector.detect( img_1, keypoints_1 );
  detector.detect( img_2, keypoints_2 );

  //-- Draw keypoints
  Mat img_keypoints_1; Mat img_keypoints_2;

  drawKeypoints( img_1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT ); 
  drawKeypoints( img_2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT ); 

  //-- Show detected (drawn) keypoints
  //imshow("Keypoints 1", img_keypoints_1 );
  //imshow("Keypoints 2", img_keypoints_2 );

	cvSaveImage("I1.png", &img_keypoints_1);
	cvSaveImage("I2.png",(const CvArr*)(&img_keypoints_2));
*/
  //waitKey(0);

  return 0;
}

/**
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }
