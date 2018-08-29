
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;

int main(int, char** argv)
{
	Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(1, 1));
	Mat img = imread("road1.jpg");
	vector<Mat> channels;
	Mat edges1;
	Mat edges;
	Mat poiskred;
	Mat poiskblue;
	RNG rng(12345);
	cvtColor(img, edges1, COLOR_RGB2HSV);
	imshow("Original", edges1);
	//split(edges1, channels);
	//imshow("1", channels[0]);
	//imshow("2", channels[1]);
	//imshow("3", channels[2]);
	inRange(edges1, Scalar(100, 178, 102), Scalar(179, 255, 255), poiskred);
	inRange(edges1, Scalar(1, 20, 20), Scalar(10, 255, 255), poiskblue);
	GaussianBlur(poiskblue, poiskblue, Size(35, 35), 5, 5);
	Canny(poiskblue, edges, 30, 60);
	dilate(edges, edges, element, Point(-1, -1), 1);
	imshow("Canny", edges);
	
	imshow("Red", poiskred);
	imshow("Blue", poiskblue);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//threshold(edges, edges, 100, 255, THRESH_BINARY);
	findContours(edges, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());


	for (size_t i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));

	}
	Mat drawing = Mat::zeros(edges.size(), CV_8UC3);
	for (size_t i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point());
		rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
		
	}
	namedWindow("Contours", WINDOW_AUTOSIZE);
	imshow("Contours", drawing);

	waitKey(0);
	return 0;
}