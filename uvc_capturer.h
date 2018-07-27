#ifndef UVC_CAPTURER_H
#define UVC_CAPTURER_H


#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

class uvc_capturer
{
public:
    uvc_capturer(int camId1, int camId2, int expInMSec);

    // JobInterface interface
public:
    void prepare();
    void loop(int thread_id);
private:
    std::vector<int> camIdx;


    int counter=0;
    cv::VideoCapture capturer;
    cv::VideoCapture capturer2;
    uint64_t timestamp;

    int m_cam_id=0;
    bool m_debugSave;
    double m_exposureTimeSec;
};

#endif // UVC_CAPTURER_H
