#pragma once

#include "../Common/Common.h"

int main()
{
    string fileName = "find_google_area.png";
    Mat src_color = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);

    Scalar lower_R(25, 0, 100);//물체를 최소한으로 남기는 rgb값
    Scalar high_R(90, 85, 200);
    Mat detect_img_R;
    inRange(src_color, lower_R, high_R, detect_img_R);//적용된 이미지

    vector<vector<Point>> contours;//컨투어가 저장되는 배열
    vector<Vec4i> hierarchy;
    findContours(detect_img_R, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);//이미지의 컨투어를 찾는다

    Mat drawing = src_color;
    cv::imshow("src_color", src_color);

    drawContours(drawing, contours, 33, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);//drawing에 컨투어를 그린다
    
    // Find the index of the largest contour
    int largestContourIndex = 0;
    double largestContourArea = cv::contourArea(contours[0]);

    for (int i = 1; i < contours.size(); i++) {//컨투어 갯수만큼 반복
        double area = cv::contourArea(contours[i]);//컨투어의 넓이
        if (area > largestContourArea) {//컨투어 가장큰 컨투어 넓이 보다 큰게 발견되면
            largestContourIndex = i;//인덱스 1저장
            largestContourArea = area;//가장큰 넓이에 넓이 저장
        }
    }


    // Compute bounding box
    Rect boundingBox = boundingRect(contours[largestContourIndex]);//컨투어의 바운딩박스 계산

    // Draw bounding box
    //rectangle(drawing, boundingBox.tl(), boundingBox.br(), Scalar(0, 255, 0), 2);
    int boundingBoxHeight = boundingBox.height;//바운딩박스 높이
    int boundingBoxWidth = boundingBox.width;//바운딩 박스 폭
    int boundingBoxArea = boundingBoxHeight * boundingBoxWidth;//사각형의 넓이
    Point boundingBoxCenter(boundingBox.x + boundingBoxWidth / 2, boundingBox.y + boundingBoxHeight / 2);//센터의 픽셀위치를 찾는다
    double ratio = double(boundingBoxWidth) / boundingBoxHeight;//비율


   

    // Rotate bounding box by 10 degrees
    float angle = -11.2f; // Rotation angle in degrees (negative for left rotation)
    float scale = 1.0f; // Scale factor (no scaling applied)
    Point2f center(static_cast<float>(boundingBoxCenter.x), static_cast<float>(boundingBoxCenter.y));








    // Convert angle to radians
    float angleRad = angle * static_cast<float>(CV_PI) / 180.0f;

    // Calculate rotated bounding box points
    float cosTheta = cos(angleRad);
    float sinTheta = sin(angleRad);
    float dx = -center.x * (cosTheta - 1.0f) + center.y * sinTheta;
    float dy = -center.y * (cosTheta - 1.0f) - center.x * sinTheta;

    Point2f rotatedPoints[4];
    rotatedPoints[0] = Point2f(boundingBox.tl().x * cosTheta - boundingBox.tl().y * sinTheta + dx, boundingBox.tl().x * sinTheta + boundingBox.tl().y * cosTheta + dy);
    rotatedPoints[1] = Point2f(boundingBox.br().x * cosTheta - boundingBox.br().y * sinTheta + dx, boundingBox.br().x * sinTheta + boundingBox.br().y * cosTheta + dy);
    rotatedPoints[2] = Point2f(boundingBox.br().x * cosTheta - boundingBox.tl().y * sinTheta + dx, boundingBox.br().x * sinTheta + boundingBox.tl().y * cosTheta + dy);
    rotatedPoints[3] = Point2f(boundingBox.tl().x * cosTheta - boundingBox.br().y * sinTheta + dx, boundingBox.tl().x * sinTheta + boundingBox.br().y * cosTheta + dy);

    // Draw rotated bounding box
    line(drawing, rotatedPoints[0], rotatedPoints[2], Scalar(255, 0, 0), 2);
    line(drawing, rotatedPoints[2], rotatedPoints[1], Scalar(255, 0, 0), 2);
    line(drawing, rotatedPoints[1], rotatedPoints[3], Scalar(255, 0, 0), 2);
    line(drawing, rotatedPoints[3], rotatedPoints[0], Scalar(255, 0, 0), 2);


    // Calculate average brightness
    Scalar meanBrightness = mean(src_color);
    






    // Convert to string
    string heightStr = "Height: " + to_string(boundingBoxHeight);
    string widthStr = "Width: " + to_string(boundingBoxWidth);
    string areaStr = "Box Area: " + to_string(boundingBoxArea);
    string centerStr = "Center: (" + to_string(boundingBoxCenter.x) + ", " + to_string(boundingBoxCenter.y) + ")";
    string ratioStr = "Ratio: " + to_string(ratio);
    string contourAreaStr = "Contour Area: " + to_string(largestContourArea);
    string boxangle = "angle: " + to_string(int(angle));
    string brightnessStr = "Avg Bright: " + to_string(meanBrightness.val[0]);
    string contourIndexStr = "contore index: " + to_string(largestContourIndex);







    // Display on the image
    putText(drawing, heightStr, Point(0, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,0, 0), 2);
    putText(drawing, widthStr, Point(0, 40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, areaStr, Point(0, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, centerStr, Point(0, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, ratioStr, Point(0, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, contourAreaStr, Point(0, 120), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, boxangle, Point(0, 140), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, brightnessStr, Point(0, 160), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
    putText(drawing, contourIndexStr, Point(boundingBoxCenter.x, ((boundingBoxCenter.y)-50)), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);


    //cmd로 출력
    std::cout << "contour index=" << largestContourIndex << endl;
    std::cout << heightStr << endl;
    std::cout << widthStr << endl;
    std::cout << areaStr << endl;
    std::cout << centerStr << endl;
    std::cout << ratioStr << endl;
    std::cout << contourAreaStr << endl;
    std::cout << boxangle << endl;
    std::cout << brightnessStr << endl;






    imshow("Contours", drawing);

    imwrite("result.jpg", drawing);


    waitKey(0);



    return 0;
}
