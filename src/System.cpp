#include "System.h"

#include <unistd.h>
#include <iostream>

#include "Common.h"
#include "JsonConfig.h"
#include "GLWindowPangolin.h"
#include "VideoSource.h"
#include "ATANCamera.h"
#include "MapMaker.h"
#include "Tracker.h"

System::System() {
    mpJsonConfig = new JsonConfig("../Config.json");
}

void System::init() {
    mpPangolinWindow = new GLWindowPangolin("slam_cg",GS::Size(640,480));

    int n_data_type = mpJsonConfig->GetInt("Data.Type");
    switch (n_data_type) {
        case 0:
            mpVideoSource = new VideoSourceOCV(mpJsonConfig->GetInt("Data.CamIdx"));
            break;
        case 1:
        {
            std::string str_dataset_dir = mpJsonConfig->GetString("Data.DatasetDir");
            mpVideoSource = new ImageDataSet(str_dataset_dir, str_dataset_dir + "/associate.txt");
        }
            break;
    }

    mpCamera = new ATANCamera();
    mpMapMaker = new MapMaker(*mpCamera);
    mpTracker = new Tracker(mpJsonConfig, mpPangolinWindow, *mpMapMaker);

    mbDone = false;
}

void System::Run() {
    if (GS::RET_FAILED == mpJsonConfig->Init())
        return;

    init();

    cv::Mat imgRGB, imgBW;
    while (!mbDone) {
        if (GS::RET_FAILED == mpVideoSource->GetFrameRGBBW(imgRGB, imgBW)) {
            std::cerr << "mpVideoSource->GetFrameRGBBW failed !!!" << std::endl;
            break;
        }

        mpPangolinWindow->SetupViewport();
        mpPangolinWindow->SetupVideoOrtho();
        mpPangolinWindow->SetupVideoRasterPosAndZoom();

        mpPangolinWindow->Clear();
        mpPangolinWindow->DrawTexture2DGray(imgBW);

        Update(imgBW, imgRGB);

        mpPangolinWindow->RenderTextureToViewport();
        mpPangolinWindow->EndFrame();

        usleep(40000);
    }
}

void System::Update(const cv::Mat &imgBW, const cv::Mat &imgRGB) {
    mpTracker->TrackFrame(imgBW, true);
}

System::~System() {
    DELETE_NEW_OBJ(mpTracker)
    DELETE_NEW_OBJ(mpMapMaker)
    DELETE_NEW_OBJ(mpCamera)
    DELETE_NEW_OBJ(mpVideoSource)
    DELETE_NEW_OBJ(mpPangolinWindow)
    DELETE_NEW_OBJ(mpJsonConfig)
}
