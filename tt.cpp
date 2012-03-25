
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
