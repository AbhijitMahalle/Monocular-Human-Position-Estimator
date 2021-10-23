/**
 * @file Detector.cpp
 * @author Driver: Abhijit Mahalle, Navigator: Hrushikesh Budhale
 * @brief library for Detector class
 * @version 0.1
 * @date 2021-10-14
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/Detector.hpp"
#include "iostream"

/**
 * @brief Default constructor for Detector
 * 
 */
Detector::Detector() {
    cv::VideoCapture camera = cv::VideoCapture();
    multiTracker = cv::MultiTracker::create();
}

/**
 * @brief Explicit constructor for Detector
 * @param camera_id int : camera id
 */
Detector::Detector(int camera_id) {
    camera = cv::VideoCapture();
    camera.open(camera_id);
    fps = camera.get(CV_CAP_PROP_FPS);
    hog_detector.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}

/**
 * @brief Constructor for a pre-recorded video stored in a file_path
 * 
 * @param file_path 
 */
Detector::Detector(std::string file_path) {
    cv::VideoCapture camera = cv::VideoCapture();
    camera.open(file_path);
    fps = camera.get(CV_CAP_PROP_FPS);
    hog_detector.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());
}

/**
 * @brief Updates private member like detections and frame by performing
 *        hog detection.
 * 
 */
void Detector::detect_humans() {
    if (!camera.isOpened()) {
        std::cout << "Cannot open the source. \n";
        return;
    }
    if (!camera.read(frame)) {
        std::cout << "\n Cannot read the frame. \n";
        return;
    }

    hog_detector.detectMultiScale(frame, detections);
}

/**
 * @brief Function that returns list of bounding boxes
 * 
 * @return std::vector<cv::Rect> 
 */
std::vector<cv::Rect> Detector::get_detections() {
    return detections;
}

/**
 * @brief Function to compute centroid of bounding boxes
 * 
 * @return std::vector<cv::Point2d> 
 */
std::vector<cv::Point2d> Detector::get_centroid() {
    std::vector<cv::Point2d> points;
    for (auto& detection : detections) {
        points.push_back(cv::Point2d(detection.x + (detection.width/2),
                               detection.y + (detection.height/2)));
    }
    return points;
}

/**
 * @brief Functions that displays output
 * 
 * @return true 
 * @return false 
 */
bool Detector::show_output() {
    bool keep_showing = true;
    for (auto& detection : detections) {
        resize_bounding_box(&detection);
        cv::rectangle(frame, detection.tl(), detection.br(),
                        cv::Scalar(255, 0, 0), 2);
    }
    cv::imshow("Frame1", frame);
    int key = cv::waitKey(static_cast<int>(1000.0/fps));
    if (key == 27 || static_cast<char>(key) == 'q') {
        cv::destroyAllWindows();
        keep_showing = false;
    }
    return keep_showing;
}

/**
 * @brief Resizes the bounding box
 * 
 * @param boxPtr Pointer to cv::Rect object created after detections
 */
void Detector::resize_bounding_box(cv::Rect* boxPtr) {
    boxPtr->x += static_cast<int>(boxPtr->width*0.15);
    boxPtr->width = static_cast<int>(boxPtr->width*0.7);
    boxPtr->y += static_cast<int>(boxPtr->height*0.1);
    boxPtr->height = static_cast<int>(boxPtr->height*0.8);
}

Detector::~Detector() {
    camera.release();
}
