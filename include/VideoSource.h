#pragma once
#ifndef __VideoSource_H
#define __VideoSource_H

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class VideoSource
{
public:
    unsigned int mWidth;
    unsigned int mHeight;
    VideoSource() {}
    virtual ~VideoSource() {}
    virtual int GetFrameRGBBW(cv::Mat &imgRGB,cv::Mat &imgBW) {return 0;}

};

class ImageDataSet : public VideoSource
{
public:
    ImageDataSet(const std::string &strDatasetDir,const std::string &strAssociationFilePath);
    ~ImageDataSet(){}
    int ReadImagesAssociationFile();
    int GetFrameRGBBW(cv::Mat &imgRGB, cv::Mat &imgBW);

private:
    std::string mStrDatasetDir;
    std::string mStrAssociationFilePath;
    std::vector<std::string> mvstrImageFilenamesRGB;
    std::vector<std::string> mvstrImageFilenamesD;
    std::vector<double> mvTimestamps;
};

class VideoSourceOCV : public VideoSource
{
public:
    VideoSourceOCV() {}
    VideoSourceOCV(int cam_idx) {
        if(cam_.isOpened())
            std::cout << "camera has opened!" << std::endl;
        if(not cam_.open(cam_idx))
            std::cerr << "Failed to open camera device!" << std::endl;
    }
    ~VideoSourceOCV() {
        if (cam_.isOpened())
            cam_.release();
    }
    int GetFrameRGBBW(cv::Mat &img_rgb, cv::Mat &img_bw) {
        cv::Mat img_bgr;
        cam_ >> img_bgr;
        cv::cvtColor(img_bgr, img_rgb, CV_BGR2RGB);
        cv::cvtColor(img_bgr, img_bw,  CV_BGR2GRAY);
    }

public:
    cv::VideoCapture cam_;
};

#endif
