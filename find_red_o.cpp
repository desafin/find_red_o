#pragma once

#include "../Common/Common.h"


int main()
{
	string fileName = "find_google_area.png";
	Mat src_color = cv::imread(fileName, cv::ImreadModes::IMREAD_COLOR);




	Scalar lower_R(25, 0, 100);
	Scalar high_R(90, 85, 200);
	Mat detect_img_R;
	inRange(src_color, lower_R, high_R, detect_img_R);



	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(detect_img_R, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	Mat drawing = src_color;


	drawContours(drawing, contours, 33, (0, 255, 0), 2, LINE_8, hierarchy, 0);

	// Compute bounding box
	Rect boundingBox = boundingRect(contours[33]);

	// Draw bounding box
	rectangle(drawing, boundingBox.tl(), boundingBox.br(), (0, 255, 0), 2);
	int boundingBoxHeight = boundingBox.height;
	int boundingBoxWidth = boundingBox.width;
	int boundingBoxArea = boundingBoxHeight * boundingBoxWidth;
	Point boundingBoxCenter(boundingBox.x + boundingBoxWidth / 2, boundingBox.y + boundingBoxHeight / 2);
	double ratio = double(boundingBoxWidth) / boundingBoxHeight;


	// Calculate contour area
	int contourArea = cv::contourArea(contours[33]);

	// Convert height and width to string
	string heightStr = "Height: " + to_string(boundingBoxHeight);
	string widthStr = "Width: " + to_string(boundingBoxWidth);
	string areaStr = "boxarea: " + to_string(boundingBoxArea);
	string centerStr = "Center: (" + to_string(boundingBoxCenter.x) + ", " + to_string(boundingBoxCenter.y) + ")";
	string ratioStr = "ratio: " + to_string(ratio);
	string contourAreaStr = "Contour Area: " + to_string(contourArea);

	// Display height and width on the image
	putText(drawing, heightStr, Point(0,20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
	putText(drawing, widthStr, Point(0,40), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
	putText(drawing, areaStr, Point(0, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
	putText(drawing, centerStr, Point(0, 80), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
	putText(drawing, ratioStr, Point(0, 100), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);
	putText(drawing, contourAreaStr, Point(0, 120), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0), 2);

	imshow("Contours", drawing);
	waitKey(0);

}