#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include "imgTransforms.h"
#include "calibration.h"

int main( /* int argc, char const *argv[] */)
{
    std::string filename = "Hands.jpg";
    //cv::Mat src = cv::imread(filename);
    //cv::Mat cvt = src;

    ImgTransforms i = ImgTransforms();
    Calibration c = Calibration();

    c.createTrackbars();
    c.on_trackbar(0, 0);
   
    //cv::cvtColor(src, cvt, cv::COLOR_BGR2HSV);


    //std::cout << "type = " << c.type2str(src.type()) << '\n';
    //std::cout << "type = " << c.type2str(cvt.type()) << '\n';

 	cv::Mat img = cv::imread(filename);
    if(img.data)
    {        
        cv::Mat hsv, cvt;

        //Isolate the hand.
        cv::bitwise_not(img, img);
        cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

        while(!c.pressedESC())
        {  c.calibrate(hsv); }
       

        cvt = c.useRange(hsv);
        std::string type = std::string( "type: " + c.type2str(cvt.type() + '\n') );
        puts ( type.c_str() );

        //cvt = cv::imdecode(cvt, cv::IMREAD_GRAYSCALE);
        i.overlayLargestContour(cvt, img);
        i.overlayAllConvexityDefects(cvt, img);

        while(!c.pressedESC())
        {
            cv::imshow("result", img);
            
        }
        return 0;
        
    }
    return 0;
}
