#ifndef IMGTRANSFORMS_H
#define IMGTRANSFORMS_H

#include "opencv2/imgproc.hpp"
#include <algorithm>

class ImgTransforms
{
    private:


    public:
    using contour_t = std::vector<cv::Point>;
    using contours_t = std::vector<contour_t>;

    using convhull_t = std::vector<cv::Point>;
    using convhulls_t = std::vector<convhull_t>;

    using convdef_t = std::vector<cv::Vec4i>;
    using convdefs_t = std::vector<convdef_t>;

    bool calibration;

    ImgTransforms()
    { }

    contours_t getAllContours(cv::Mat& filteredImage) const
    {
        contours_t contours;
        std::vector<cv::Vec4i> hierarchy;
	    cv::findContours(filteredImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        return contours;
    }

    contour_t getLargestContour(cv::Mat& filteredImage) const
    {
        contours_t contours = getAllContours(filteredImage);

        double largestArea = 0;
        contour_t largestContour = contour_t();

        for(const auto& contour : contours)
        {
            double a = cv::contourArea(contour);
            if( a > largestArea )
            {
                largestArea = a;
                largestContour = contour;
            }
        }

        return largestContour;
    }

    convhull_t getConvexHull(const contour_t& contour) const
    {
        convhull_t hull = convhull_t(contour.size());
        cv::convexHull( cv::Mat(contour), hull);
        return hull;
    }

    convdef_t getConvexityDefects(const contour_t& contour) const
    {
        convdef_t defs = convdef_t(contour.size());
        
        std::vector<int> hullIDs = std::vector<int>(contour.size());
        cv::convexHull( contour, hullIDs, false, false);

        if(hullIDs.size() > 3)
        { 
            cv::convexityDefects( contour, hullIDs, defs);
        }

        return defs;
    }

    void overlayAllContours(const contours_t& contours, cv::Mat& imageToOverlay, 
                            cv::Scalar drawColour = CV_RGB(255,255,255), int drawThickness = 2 ) const
    {
        cv::drawContours(imageToOverlay, contours, -1, drawColour, drawThickness);
    }

    void overlayAllContours(cv::Mat& filteredImage, cv::Mat& imageToOverlay, 
                            cv::Scalar drawColour = CV_RGB(255,255,255), int drawThickness = 2 ) const
    {
        contours_t contours = getAllContours(filteredImage);
        cv::drawContours(imageToOverlay, contours, -1, drawColour, drawThickness);
    }



    void overlayLargestContour(const contour_t& contour, cv::Mat& imageToOverlay,
                               cv::Scalar drawColour = CV_RGB(255,255,255), int drawThickness = 2 ) const
    {
        cv::drawContours(imageToOverlay, contours_t{ contour }, -1, drawColour, drawThickness );
    }

    void overlayLargestContour(cv::Mat& filteredImage, cv::Mat& imageToOverlay,
                               cv::Scalar drawColour = CV_RGB(255,255,255), int drawThickness = 2 ) const
    {
        contour_t contour = getLargestContour(filteredImage);
        cv::drawContours(imageToOverlay, contours_t{ contour }, -1, drawColour, drawThickness );
    }



    void overlayAllConvexityDefects(cv::Mat filteredImage, cv::Mat imageToOverlay,
                                    cv::Scalar BdrawColour = CV_RGB(255,0,0), 
                                    cv::Scalar EdrawColour = CV_RGB(0,255,0), 
                                    cv::Scalar FdrawColour = CV_RGB(0,0,255), int drawThickness = 2 ) const
    {
        contours_t contours = getAllContours(filteredImage);    

        for(const auto& contour : contours)
        {
            convhull_t hulls = getConvexHull( contour );

            convdef_t defects = getConvexityDefects(contour);

            for(const auto& defect : defects)
            {
                int defStart = defect.val[0]; cv::Point defectCurveStartPoint(contour[defStart]);
                int defEnd =   defect.val[1];   cv::Point defectCurveEndPoint(contour[defEnd]);
                int defFar =   defect.val[2];   cv::Point defectCurveFarPoint(contour[defFar]);

                //double depth = defect.val[3] / 256.0;

                cv::circle( imageToOverlay, defectCurveStartPoint, 3, BdrawColour, drawThickness);
                cv::circle( imageToOverlay, defectCurveEndPoint, 3, EdrawColour, drawThickness);
                cv::circle( imageToOverlay, defectCurveFarPoint, 3, FdrawColour, drawThickness);
            }

             cv::drawContours(imageToOverlay, convhulls_t{hulls}, -1, BdrawColour);

        }
    }

    void overlayLargestConvexityDefect(cv::Mat filteredImage, cv::Mat imageToOverlay,
                                       cv::Scalar BdrawColour = CV_RGB(255,0,0), 
                                       cv::Scalar EdrawColour = CV_RGB(0,255,0), 
                                       cv::Scalar FdrawColour = CV_RGB(0,0,255), int drawThickness = 2 ) const
    {
        contour_t contour = getLargestContour(filteredImage);
        convhull_t hull = getConvexHull( contour );
        convdef_t defects = getConvexityDefects(contour);

        for(const auto& defect : defects)
            {
                int defStart = defect.val[0]; cv::Point defectCurveStartPoint(contour[defStart]);
                int defEnd = defect.val[1];   cv::Point defectCurveEndPoint(contour[defEnd]);
                int defFar = defect.val[2];   cv::Point defectCurveFarPoint(contour[defFar]);

               //double depth = defect.val[3] / 256.0;

                cv::circle( imageToOverlay, defectCurveStartPoint, 3, BdrawColour, drawThickness);
                cv::circle( imageToOverlay, defectCurveEndPoint, 3, EdrawColour, drawThickness);
                cv::circle( imageToOverlay, defectCurveFarPoint, 3, FdrawColour, drawThickness);
            }
    }


};


#endif