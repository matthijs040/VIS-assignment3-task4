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

 	cv::VideoCapture cap(filename);
    if(cap.isOpened())
    {
        
        cv::Mat frame, hsvframe, cvtframe;


	    while ( cap.read(frame) )
        {
            //Isolate the hand.
            cv::bitwise_not(frame, frame);
            cv::cvtColor(frame, hsvframe, cv::COLOR_BGR2HSV);

            while( !c.pressedESC() )
            { c.calibrate(hsvframe); }
            cvtframe = c.useRange(hsvframe);

            std::string type = std::string( "type: " + c.type2str(cvtframe.type() + '\n') );
            puts ( type.c_str() );

            //cvtframe = cv::imdecode(cvtframe, cv::IMREAD_GRAYSCALE);



            i.overlayLargestContour(cvtframe, frame);


            

            i.overlayAllConvexityDefects(cvtframe, frame);

            while(!c.pressedESC())
            cv::imshow("result", frame);
        }
    }

    return 0;
}
