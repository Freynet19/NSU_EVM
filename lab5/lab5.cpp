#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

int swapFaces(const cv::Mat& img) {
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load("/usr/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
        return -1;
    }

    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(img, faces, 1.2, 5, 0, cv::Size(30, 30));

    if (faces.size() >= 2) {
        cv::Mat face1 = img(faces[0]);
        cv::Mat face2 = img(faces[1]);

        cv::Mat resized_face1, resized_face2;
        resize(face1, resized_face1, faces[1].size());
        resize(face2, resized_face2, faces[0].size());

        resized_face1.copyTo(img(faces[1]));
        resized_face2.copyTo(img(faces[0]));
    }
    return static_cast<int>(faces.size());
}

int main() {
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error opening video stream" << std::endl;
        return -1;
    }

    cv::Mat frame;
    int frameCount = 0;
    auto start = std::chrono::system_clock::now();
    std::string fpsText, currentTime;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        std::string faceCountText = "Face count: " + std::to_string(swapFaces(frame));

        frameCount++;
        if (frameCount % 25 == 0) {
            auto current = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = current - start;
            start = current;
            double fps = frameCount / elapsed.count();
            frameCount = 0;
            fpsText = "FPS: " + std::to_string(fps);
        }

        cv::putText(frame, fpsText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(95, 230, 32), 3);
        cv::putText(frame, faceCountText, cv::Point(10, 60), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(95, 230, 32), 3);

        imshow("FaceSwap", frame);

        if (cv::waitKey(30) >= 0) break;
    }

    return 0;
}
