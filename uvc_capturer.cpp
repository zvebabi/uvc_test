#include "uvc_capturer.h"
#include <boost/filesystem.hpp>
#include <thread>
#include <iostream>
uvc_capturer::uvc_capturer(int camId1, int camId2, int expInMSec)

{
    camIdx.resize(2);
    camIdx[0] = camId1;
    camIdx[1] = camId2;
    m_exposureTimeSec = expInMSec/1000.0;
}

void uvc_capturer::prepare()
{
//    if(m_debugSave)
    {
        boost::filesystem::path dir("uvc_cam1");
        boost::filesystem::create_directory(dir);
        boost::filesystem::path dir2("uvc_cam2");
        boost::filesystem::create_directory(dir2);
    }
    //try to open uvc cam with specific backend
    int apiID = cv::CAP_V4L2;
    capturer.open(camIdx[0]  + apiID);
    capturer2.open(camIdx[1] + apiID);
    if(!capturer.isOpened() ||!capturer2.isOpened()  )
    {
        std::cout << "***Could not initialize capturing...***";
        return;
    }
//    capturer.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    capturer.set(cv::CAP_PROP_EXPOSURE, m_exposureTimeSec); //in seconds
    capturer.set(cv::CAP_PROP_FRAME_WIDTH,1280);
    capturer.set(cv::CAP_PROP_FRAME_HEIGHT,960);
//    capturer.set(cv::CAP_PROP_TRIGGER, 1);
//    std::cout << "cap_temp: " << capturer.get(cv::CAP_PROP_TEMPERATURE);
//    std::cout << "UVC camera 1 exposure send " << 1000 * m_exposureTimeSec                   << " ms";
//    std::cout << "UVC camera 1 exposure read " << 1000 * capturer.get(cv::CAP_PROP_EXPOSURE) << " ms";
//    std::cout << "UVC camera 1 resolution: " << capturer.get(cv::CAP_PROP_FRAME_WIDTH) << " x "
//                                                  << capturer.get(cv::CAP_PROP_FRAME_HEIGHT);
//    capturer2.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
    capturer2.set(cv::CAP_PROP_EXPOSURE, m_exposureTimeSec); //in seconds
    capturer2.set(cv::CAP_PROP_FRAME_WIDTH,1280);
    capturer2.set(cv::CAP_PROP_FRAME_HEIGHT,960);
//    capturer.set(cv::CAP_PROP_TRIGGER, 1);
//    std::cout << "cap_temp: " << capturer.get(cv::CAP_PROP_TEMPERATURE);
//    std::cout << "UVC camera 2 exposure send " << 1000 * m_exposureTimeSec                   << " ms";
//    std::cout << "UVC camera 2 exposure read " << 1000 * capturer.get(cv::CAP_PROP_EXPOSURE) << " ms";
//    std::cout << "UVC camera 2 resolution: " << capturer.get(cv::CAP_PROP_FRAME_WIDTH) << " x "
//                                                  << capturer.get(cv::CAP_PROP_FRAME_HEIGHT);

}

void uvc_capturer::loop(int thread_id)
{
//    int index = lock();
//    if (index == -1)
//    {
//        std::this_thread::yield();
//        return;
//    }

//    std::vector<std::shared_ptr<ImageInterface>> imgData = getImages(index);
//    if (timestamp <= imgData[0]->getTimeStamp())
//    {
//        timestamp = imgData[0]->getTimeStamp();
    while(1)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        cv::Mat temp, temp2;
        cv::Mat imgToShow(960, 1280*2,CV_8UC3);
        auto th2= [&](){capturer2 >> temp2;};
        auto th1 = [&](){capturer >> temp;};
        std::vector<std::thread> pool;
        pool.push_back(std::thread(th2));
        pool.push_back(std::thread(th1));
        for(int i=0; i< pool.size(); i++)
            pool[i].join();

        if (!temp.empty() && !temp2.empty())
        {
            temp.copyTo(imgToShow(cv::Rect(0,0,1280,960)));
            temp2.copyTo(imgToShow(cv::Rect(1280,0,1280,960)));
//            if(m_debugSave)
            {
                char buf[100];
                sprintf(buf, "%09d", counter);
                std::string name1 = std::string("uvc_cam1/") + buf + std::string(".jpg");
                std::vector<int> params;
                params.push_back(cv::IMWRITE_JPEG_QUALITY);
                params.push_back(100);
                cv::imwrite(name1, temp, params);

                std::string name2 = std::string("uvc_cam2/") + buf + std::string(".jpg");
                cv::imwrite(name2, temp2, params);
            }
//            if (counter >5)
//            {
//                cv::imshow("Output", imgToShow);
//                cv::waitKey(0);
//            }
        }
        else
        {
            continue;
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        int delay = 100 - std::chrono::duration_cast<std::chrono::milliseconds>(t2- t1).count();
        counter++;
        std::cout << counter << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay > 0 ? delay : 0));

    }
//    unlock(index);
//    counter++;
}
