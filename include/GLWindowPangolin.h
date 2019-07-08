#pragma once
#ifndef __GLWindowPangolin_H
#define __GLWindowPangolin_H

#include "types.h"

#include <string>
#include <vector>

#include <GL/glew.h>
#include <pangolin/pangolin.h>
#include <opencv2/core/types.hpp>
#include <opencv2/core/mat.hpp>

class GLWindowPangolin
{
public:
    GLWindowPangolin(){}
    GLWindowPangolin(const std::string title, cg::Size sizeWindow);

    void SetupViewport();
    void SetupVideoOrtho();
    void SetupVideoRasterPosAndZoom();

    bool LoadTextures(cv::Mat matImg, GLuint texture);
    void DrawTexture2DGray(const cv::Mat &imgGray);
    void DrawTexture2DRGB(const cv::Mat &imgRGB);
    void RenderTextureToViewport();

    void DrawPoints2D(const std::vector<cv::Point2i> &points, cg::RGB rgb, float size);
    void DrawLines(const cv::Point2i &ptStart, cg::RGB rgbStart,
                   const cv::Point2i &ptEnd  , cg::RGB rgbEnd,
                   float width);
    void DrawOrigeAxis();

    inline void Clear(){ glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }
    inline void EndFrame(){ pangolin::FinishFrame(); }

private:
    cg::Size mSizeWin;
    cg::Size mSizeVideo;
    cg::RGB mRGB;
    pangolin::GlTexture mTexture;
};
#endif
