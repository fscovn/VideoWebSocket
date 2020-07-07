/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   vs_processing.h
 * Author: ubuntu
 *
 * Created on May 25, 2020, 11:13 AM
 */

#ifndef VS_PROCESSING_H
#define VS_PROCESSING_H
#include <opencv2/opencv.hpp>
#include "base64.h"
using namespace cv;
using namespace std;

class vs_processing {
public:
    vs_processing();
    virtual ~vs_processing();
    
    static string to_base64(Mat img) ;
    static float get_video_fps(VideoCapture videoCapture) {
        if (!videoCapture.isOpened())
            return 0;
        return videoCapture.get(CAP_PROP_FPS);
    }
    static double get_video_frame_count(VideoCapture videoCapture){
        if (!videoCapture.isOpened())
            return 0;
        double count = videoCapture.get(CAP_PROP_FRAME_COUNT);
        return count;
    }
private:

};

#endif /* VS_PROCESSING_H */

