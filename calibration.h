#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include "string"



class Calibration
{
    private:

    bool calibrating = true;
    int H_MIN;   int H_MAX; int S_MIN; 
    int V_MIN;   int V_MAX; int S_MAX;

    int kerode; 
    int kdilate;

    int kblur;
    int threshval = 0;





    public:

    Calibration()
    {
        H_MIN = 0;   S_MIN = 0;    V_MIN = 80;
        H_MAX = 255; S_MAX = 255; V_MAX = 255; 

        kerode = 1;
        kdilate = 1;
        kblur = 1;
    }

    static void on_trackbar(int, void*)
    {
    }


    bool pressedESC()
    {
        char c=(char)cv::waitKey(100);
        if(c==27)
        return true;

        return false;
    }

    std::string type2str(int type) 
    {
        std::string r;

        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);

        switch ( depth ) 
        {
            case CV_8U:  r = "8U"; break;
            case CV_8S:  r = "8S"; break;
            case CV_16U: r = "16U"; break;
            case CV_16S: r = "16S"; break;
            case CV_32S: r = "32S"; break;
            case CV_32F: r = "32F"; break;
            case CV_64F: r = "64F"; break;
            default:     r = "User"; break;
        }

        r += "C";
        r += (chans+'0');

        return r;
    }


    // NOTE: This function has to run in a loop body due to complications with function scope.
    inline void calibrate(cv::Mat& img)
    {
        cv::Mat result;
	    //cv::cvtColor(og, img, cv::COLOR_BGR2HSV);
   
        cv::inRange(img, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), result);
        cv::imshow("calibration-window", result);
      
    }

    inline cv::Mat useRange(cv::Mat img)
    {
        cv::Mat out;
        cv::inRange(img, cv::Scalar(H_MIN, S_MIN, V_MIN), cv::Scalar(H_MAX, S_MAX, V_MAX), out);       
        return out;
    }


    inline void morphit(cv::Mat &img)
    {
    	cv::erode(img, img,  cv::getStructuringElement( cv::MORPH_RECT, cv::Size(kerode, kerode)));
    	cv::dilate(img, img, cv::getStructuringElement( cv::MORPH_RECT, cv::Size(kdilate, kdilate)));
    }

    inline void blurthresh(cv::Mat &img)
    {
    	//medianBlur(img,img,kblur%2+3+kblur);
    	cv::blur     (img, img, cv::Size(kblur, kblur), cv::Point(-1, -1), cv::BORDER_DEFAULT);
    	cv::threshold(img, img, threshval, 255, cv::THRESH_BINARY_INV);
    }

    inline void createTrackbars()
    {
	    std::string trackbarWindowName = "TrackBars";
	    cv::namedWindow(trackbarWindowName, cv::WINDOW_NORMAL);
	    cv::createTrackbar("H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar);
	    cv::createTrackbar("H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar);
	    cv::createTrackbar("S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar);
	    cv::createTrackbar("S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar);
	    cv::createTrackbar("V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar);
	    cv::createTrackbar("V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar);
	    cv::createTrackbar("Erode", trackbarWindowName, &kerode, 31, on_trackbar);
	    cv::createTrackbar("Dilate", trackbarWindowName, &kdilate, 31, on_trackbar);
	    cv::createTrackbar("Blur", trackbarWindowName, &kblur, 255, on_trackbar);
	    cv::createTrackbar("Thresh", trackbarWindowName, &threshval, 255, on_trackbar);

    }
    

};


