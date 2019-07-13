#pragma once
#ifndef __Tracker_H
#define __Tracker_H

#include "ImgProc.h"
#include "KeyFrame.h"

#include <list>
#include <sophus/se3.h>

class JsonConfig;
class GLWindowPangolin;
class MapMaker;

struct Trail
{
    MiniPatch mPatch;
    cv::Point2i ptCurrentPos;
    cv::Point2i ptInitialPos;
};

class Tracker
{
public:
    Tracker(JsonConfig *pJsonConfig, GLWindowPangolin *pWindowPangolin, MapMaker &mapmaker);
    ~Tracker(){}

    void TrackFrame(const cv::Mat &imgBW, bool bDraw);

private:
    bool mbDraw;
    int id_frame_;
    int id_keyframe_last_;
    KeyFrame kf_first_;
    KeyFrame kf_c_; // current key frame
    KeyFrame kf_p_; // previous key frame
    std::list<Trail> mlTrails;
    enum{TRAIL_TRACKING_NOT_STARTED,TRAIL_TRACKING_STARTED,TRAIL_TRACKING_COMPLETE} mnInitialStage;

    JsonConfig *mpJsonConfig;
    GLWindowPangolin *mpPangolinWindow;
    MapMaker &mMapMaker;
    Sophus::SE3 se3_cw_;

    void TrailTracking_Start();
    int TrailTracking_Advance();

    void Reset();
};
#endif
