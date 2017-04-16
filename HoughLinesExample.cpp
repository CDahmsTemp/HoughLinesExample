// CannyStill.cpp

#include <opencv2/opencv.hpp>

#include<iostream>
#include<conio.h>           // may have to modify this line if not using Windows

// global constants ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLUE = cv::Scalar(255.0, 0.0, 0.0);

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {

    cv::Mat imgOriginal;            // original image read in from file
    cv::Mat imgCanny;               // Canny edges image of original, should be a 1-channel binary image
    cv::Mat imgHoughLines;          // Canny edges image, changed to 3-channel and with the HoughLines() lines drawn on it
    cv::Mat imgHoughLinesP;         // Canny edges image, changed to 3-channel and with the HoughLinesP() lines drawn on it

    // load the original image from file, show an error message and bail if not successful
    imgOriginal = cv::imread("building.png");
    if (imgOriginal.empty()) {
        std::cout << "error: image not read from file\n\n";
        _getch();           // may have to modify this line if not using Windows
        return(0);
    }

    // get the Canny edges of the original image and show for reference
    cv::Canny(imgOriginal, imgCanny, 100, 200);
    cv::imshow("imgCanny", imgCanny);

    // for demonstration purposes, first we'll find and show lines with the function HoughLines(),
    // then we'll find and show lines again with HoughLinesP(), usually HoughLinesP() is the better choice (see the rest of the program for clarification)

    // find lines using HoughLines()
    std::vector<cv::Vec2f> houghLines;
    cv::HoughLines(imgCanny, houghLines, 1, CV_PI / 180.0, 200);

    // init imgHoughLines to be the same as the Canny image, but converted to 3-channel (BGR color) so we can draw the imgHoughLines lines on it next
    cv::cvtColor(imgCanny, imgHoughLines, CV_GRAY2BGR);

    // for each line found with HoughLinesP(), draw the line on the applicable image
    for (auto &houghLine : houghLines) {
        // the function HoughLines() yields lines in the format of distance from the origin in pixels (a.k.a. radius or r) and rotation angle theta in radians, however
        // the line drawing function line() takes two points, therefore we have to convert the radius / angle to two points so we can draw each line
        // note that HoughLinesP() yields lines in the format of two points which is usually easier to work with, also HoughLinesP() seems more efficient,
        // therefore in most cases HoughLinesP() is the better choice
        float distFromOrigin = houghLine[0];
        float theta = houghLine[1];
        cv::Point point1;
        cv::Point point2;
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a * distFromOrigin;
        double y0 = b * distFromOrigin;
        point1.x = cvRound(x0 + 1000 * (-b));
        point1.y = cvRound(y0 + 1000 * (a));
        point2.x = cvRound(x0 - 1000 * (-b));
        point2.y = cvRound(y0 - 1000 * (a));
        cv::line(imgHoughLines, point1, point2, SCALAR_BLUE, 3);
    }

    cv::imshow("imgHoughLines", imgHoughLines);     // finally we can show the image with the HoughLines()

    // now, find and show lines with HoughLinesP()

    // find lines using HoughLinesP()
    std::vector<cv::Vec4i> houghLinesP;
    cv::HoughLinesP(imgCanny.clone(), houghLinesP, 1, CV_PI / 180.0, 200, 30, 10);

    // init imgHoughLinesP to be the same as the Canny image, but converted to 3-channel (BGR color) so we can draw the HoughLinesP lines on it next
    cv::cvtColor(imgCanny, imgHoughLinesP, CV_GRAY2BGR);

    // for each line found with HoughLinesP(), draw the line on the applicable image
    for (auto &houghLineP : houghLinesP) {
        cv::line(imgHoughLinesP, cv::Point(houghLineP[0], houghLineP[1]), cv::Point(houghLineP[2], houghLineP[3]), SCALAR_BLUE, 3);
    }

    cv::imshow("imgHoughLinesP", imgHoughLinesP);       // finally we can show the image with the HoughLinesP()

    cv::waitKey(0);                 // hold windows open until user presses a key

    return(0);
}
