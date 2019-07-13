#include "Tracker.h"

#include <utility>

#include "JsonConfig.h"
#include "GLWindowPangolin.h"
#include "MapMaker.h"

Tracker::Tracker(JsonConfig *pJsonConfig, GLWindowPangolin *pWindowPangolin, MapMaker &mapmaker):
    mpJsonConfig(pJsonConfig),
    mpPangolinWindow(pWindowPangolin),
    mMapMaker(mapmaker)
{
    kf_c_.bFixed = false;
    Reset();
}

void Tracker::TrackFrame(const cv::Mat &imgBW, bool bDraw)
{
    mbDraw = bDraw;
    kf_c_.MakeKeyFrame_Lite(imgBW);

    id_frame_++;

    if(mbDraw) {
        mpPangolinWindow->DrawPoints2D(kf_c_.aLevels[0].vCorners, cg::RGB(1, 0, 1), 1.0f);
    }

    // mono slam init stage
    if(mnInitialStage == TRAIL_TRACKING_NOT_STARTED)
    {
        if(id_frame_==5)
        {
            TrailTracking_Start();
            mnInitialStage = TRAIL_TRACKING_STARTED;
        }
        return;
    }
    if(mnInitialStage == TRAIL_TRACKING_STARTED)
    {
        int nGoodTrails = TrailTracking_Advance();
        if(nGoodTrails < 10)
        {
            Reset();
            return;
        }
        if(id_frame_==12)
        {
            std::vector<std::pair<cv::Point2i, cv::Point2i> > matches;
            for(std::list<Trail>::iterator i = mlTrails.begin(); i!=mlTrails.end(); i++)
            {
                matches.push_back(std::pair<cv::Point2i, cv::Point2i>(i->ptInitialPos,i->ptCurrentPos));
            }
            mMapMaker.InitFromStereo(kf_first_, kf_c_, matches, se3_cw_);
            id_keyframe_last_ = id_frame_;
            mnInitialStage = TRAIL_TRACKING_COMPLETE;
        }
    }
    if(mnInitialStage != TRAIL_TRACKING_COMPLETE)
        return;

    // TODO[cg]: tracking

    bool is_kf_c1 = (id_frame_ - id_keyframe_last_) > 5;

    if(is_kf_c1) {

    }

    id_keyframe_last_ = id_frame_;
}

void Tracker::TrailTracking_Start()
{
    kf_c_.MakeKeyFrame_Rest();
    kf_first_ = kf_c_;

    std::vector<std::pair<double,cv::Point2i> > vCornersAndSTScores;
    for(unsigned int i=0; i<kf_c_.aLevels[0].vCandidates.size(); i++)
    {
        Candidate &c = kf_c_.aLevels[0].vCandidates[i];
        vCornersAndSTScores.push_back(std::pair<double,cv::Point2i>(c.dSTScore, c.ptLevelPos));
    }

    std::stable_sort(vCornersAndSTScores.begin(), vCornersAndSTScores.end(),
                     [](const std::pair<double,cv::Point2i> &a,const std::pair<double,cv::Point2i> &b){return a.first > b.first;});

    int nToAdd = mpJsonConfig->GetInt("Tracker.MaxInitialTrails");
    for(unsigned int i = 0; i<vCornersAndSTScores.size() && nToAdd > 0; i++)
    {
        Trail t;
        t.mPatch.SampleFromImage(kf_c_.aLevels[0].im, vCornersAndSTScores[i].second);
        t.ptInitialPos = vCornersAndSTScores[i].second;
        t.ptCurrentPos = t.ptInitialPos;
        mlTrails.push_back(t);
        nToAdd--;
    }

    kf_p_ = kf_first_;
}

int Tracker::TrailTracking_Advance()
{
    int nGoodTrails = 0;
    int nMaxSSD = mpJsonConfig->GetInt("Tracker.MiniPatchMaxSSD");
    MiniPatch BackwardsPatch;
    Level &lCurrentFrame = kf_c_.aLevels[0];
    Level &lPreviousFrame = kf_p_.aLevels[0];

    for(std::list<Trail>::iterator i = mlTrails.begin(); i!=mlTrails.end();)
    {
        std::list<Trail>::iterator next = i;
        next++;
        Trail &trail = *i;
        cv::Point2i ptStart = trail.ptCurrentPos;
        cv::Point2i ptEnd = ptStart;
        int nFound = trail.mPatch.FindPatch(ptEnd, lCurrentFrame.im, 10, lCurrentFrame.vCorners, nMaxSSD);
        if(nFound == cg::RET_SUCESS)
        {
            BackwardsPatch.SampleFromImage(lCurrentFrame.im, ptEnd);
            cv::Point2i ptBackWardsFound = ptEnd;
            nFound = BackwardsPatch.FindPatch(ptBackWardsFound, lPreviousFrame.im, 10, lPreviousFrame.vCorners, nMaxSSD);
            cv::Point2i diffPts = ptBackWardsFound - ptStart;
            if(diffPts.dot(diffPts) > 2)
                nFound = cg::RET_FAILED;
            trail.ptCurrentPos = ptEnd;
            nGoodTrails++;
        }
        if(mbDraw)
        {
            cg::RGB rgbStart, rgbEnd;
            if(nFound == cg::RET_SUCESS)
            {
                rgbStart = cg::RGB(1,1,0);
                rgbEnd   = cg::RGB(1,0,0);
            }
            else
            {
                rgbStart = rgbEnd = cg::RGB(0,1,1);
            }
            mpPangolinWindow->DrawLines(trail.ptInitialPos, rgbStart, trail.ptCurrentPos, rgbEnd, 2.0f);
        }
        if(nFound == cg::RET_FAILED)
        {
            mlTrails.erase(i);
        }
        i = next;
    }

    kf_p_ = kf_c_;
    return nGoodTrails;
}

void Tracker::Reset()
{
    id_frame_ = 0;
    id_keyframe_last_ = 0;
    mnInitialStage = TRAIL_TRACKING_NOT_STARTED;
    mlTrails.clear();
}
