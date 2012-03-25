/*
 *  A simple FastCGI application example in C++.
 *
 *  $Id: echo-cpp.cpp,v 1.10 2002/02/25 00:46:17 robs Exp $
 *
 *  Copyright (c) 2001  Rob Saccoccio and Chelsea Networks
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif
#include "fcgio.h"
#include "fcgi_config.h"  // HAVE_IOSTREAM_WITHASSIGN_STREAMBUF


//#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

// Maximum number of bytes allowed to be read from stdin
/**
 * @file SURF_detector
 * @brief SURF keypoint detection + keypoint drawing with OpenCV functions
 * @author A. Huaman
 */

void readme();

/**
 * @function main
 * @brief Main function
 * @function readme
 */
void readme()
{ std::cout << " Usage: ./SURF_detector <img1> <img2>" << std::endl; }

static const unsigned long STDIN_MAX = 1000000;

static void penv(const char * const * envp)
{
    cout << "<PRE>\n";
    for ( ; *envp; ++envp)
    {
        cout << *envp << "\n";
    }
    cout << "</PRE>\n";
}

static long gstdin(FCGX_Request * request, char ** content)
{
    char * clenstr = FCGX_GetParam("CONTENT_LENGTH", request->envp);
    unsigned long clen = STDIN_MAX;

    if (clenstr)
    {
        clen = strtol(clenstr, &clenstr, 10);
        if (*clenstr)
        {
            cerr << "can't parse \"CONTENT_LENGTH="
                 << FCGX_GetParam("CONTENT_LENGTH", request->envp)
                 << "\"\n";
            clen = STDIN_MAX;
        }

        // *always* put a cap on the amount of data that will be read
        if (clen > STDIN_MAX) clen = STDIN_MAX;

        *content = new char[clen];

        cin.read(*content, clen);
        clen = cin.gcount();
    }
    else
    {
        // *never* read stdin when CONTENT_LENGTH is missing or unparsable
        *content = 0;
        clen = 0;
    }

    // Chew up any remaining stdin - this shouldn't be necessary
    // but is because mod_fastcgi doesn't handle it correctly.

    // ignore() doesn't set the eof bit in some versions of glibc++
    // so use gcount() instead of eof()...
    do cin.ignore(1024); while (cin.gcount() == 1024);

    return clen;
}

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

int main (void)
{
		cout << "hello world!" << endl;
    int count = 0;
    long pid = getpid();

    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0)
    {
        // Note that the default bufsize (0) will cause the use of iostream
        // methods that require positioning (such as peek(), seek(),
        // unget() and putback()) to fail (in favour of more efficient IO).
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);
				
#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
        cin  = &cin_fcgi_streambuf;
        cout = &cout_fcgi_streambuf;
        cerr = &cerr_fcgi_streambuf;
#else
        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);
#endif

        // Although FastCGI supports writing before reading,
        // many http clients (browsers) don't support it (so
        // the connection deadlocks until a timeout expires!).
        char * content;
        //unsigned long clen = gstdin(&request, &content);
				
        //penv(request.envp);
        
				cout << "<H4>Process/Initial Environment</H4>\n";
				
				//penv(environ);
				
       // if (clen) cout.write(content, clen);
			  
				std::string sbuff;
				/*char* cp = content;
				int i = 0;
				while (i < clen){
				  sbuff += *cp;
					cp++;
					i++;
				}
					*/
				int index = 0;
			
				#if 0
				if ( sbuff.find("image/bmp") != string::npos ) {
				  cout << "Has image and contend: <br>";
					index = sbuff.find("image/bmp") + std::string("image/bmp").length()+4;
				  sbuff = sbuff.substr(sbuff.find("image/bmp"));
					//cout << sbuff << endl;
					//cout << "HHHHHHHHHHHHHHHHHHHHHHHHHH" << endl;
					cout << "HHHHH" << index << "HHHHHH" << endl;
				  sbuff = sbuff.substr(std::string("image/bmp").length(),sbuff.find("------WebKit")-std::string("------WebKit").length());
					/*cout << "<textarea cols=\"150\" rows=\"50\" > ";
					cout << sbuff;
					cout << "</textarea>";*/
					//cout.write(sbuff.c_str(),10000);
				}else{
					cout << "no image\n" ;	
				}
       
				bool bhasjpg = false;
				if ( sbuff.find("image/jpeg") != string::npos ) {
				  //cout << "Has image and contend: <br>";
					index = sbuff.find("image/jpeg") + std::string("image/jpeg").length()+4;
				  sbuff = sbuff.substr(sbuff.find("image/jpeg"));
					//cout << sbuff << endl;
					int co = 50;
					//cout << "HHHHH" << index << "HHHHHH" << endl;
				  sbuff = sbuff.substr(std::string("image/jpeg").length(),sbuff.find("------WebKit")-std::string("------WebKit").length());
					//cout << "<textarea cols=\"150\" rows=\"50\" ";
					//cout << sbuff;
					//cout << "</textarea>";
					//cout.write(sbuff.c_str(),10000);
					bhasjpg = true;
				}else{
					cout << "no image\n" ;	
				}

				#endif
				/*if (bhasjpg)
				{
						std::vector<unsigned char> buf;
						for (int j = 0; j < sbuff.length(); j++){
							buf.push_back(content[j+index]);
						}
						
						Mat buf_loaded = imdecode(Mat(buf),-1);
						if(buf_loaded.empty()){
							cout << "<br> Decoding failed with jpg! <br>";
						}else{
							cout << "Decoding successful!" ;
							cout << "PicSize: " << buf_loaded.rows << " x " << buf_loaded.cols << "<br>" ;
						}

				}*/
        //if (content) delete []content;
				
				//ImgMatch(img1,img2,count);
				// If the output streambufs had non-zero bufsizes and
        // were constructed outside of the accept loop (i.e.
        // their destructor won't be called here), they would
        // have to be flushed here.
    }

#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
    cin  = cin_streambuf;
    cout = cout_streambuf;
    cerr = cerr_streambuf;
#else
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);
#endif

    return 0;
}
