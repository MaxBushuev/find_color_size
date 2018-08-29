#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using std::vector;
int main()
{
	Mat img;// = imread("Flying Vinni.jpg");
	Mat hsv;
	Mat poiskred;

	int findhred=0;
	int findsred=0;
	int findvred=0;
	int findhblue=0;
	int findsblue=0;
	int findvblue=0;

	int lengthmin=0;
	int lengthmax=0;

	int sootmin = 50;
	int sootmax = 50;

	float sootmind;
	float sootmaxd;
	VideoCapture cap(0);
	
	while (true)
	{

		cap >> img;
		cvtColor(img, hsv, COLOR_RGB2HSV);
		createTrackbar("Hmin", "Red", &findhred, 180);
		createTrackbar("Smin", "Red", &findsred, 255);
		createTrackbar("Vmin", "Red", &findvred, 255);
		createTrackbar("Hmax", "Red", &findhblue, 180);
		createTrackbar("Smax", "Red", &findsblue, 255);
		createTrackbar("Vmax", "Red", &findvblue, 255);

		createTrackbar("Lengthmin", "Result", &lengthmin, 1000);
		createTrackbar("sootmin", "Result", &sootmin, 180);
		createTrackbar("Lenghtmax", "Result", &lengthmax, 1000);
		createTrackbar("sootmax", "Result", &sootmax, 180);
		sootmind = sootmin / 100;
		sootmaxd = sootmax / 100;
		inRange(hsv, Scalar(findhred, findsred, findvred), Scalar(findhblue, findsblue, findvblue), poiskred);
		imshow("Red", poiskred);
		vector<vector<Point>> contours;
		
		findContours(poiskred, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0,0));
		vector<vector<Point> > contours_poly(contours.size() + 1);
		vector<Rect> boundRect(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));

		}
		for (size_t i = 0; i < contours.size(); i++)
		{
			if (boundRect[i].height > lengthmin && boundRect[i].width > lengthmin&& boundRect[i].height < lengthmax && boundRect[i].width < lengthmax && sootmind < boundRect[i].height / boundRect[i].width < sootmaxd)
			{
				drawContours(poiskred, contours_poly, (int)i, Scalar(0, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
				rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2, 8, 0);
			}
		}
		imshow("Result", poiskred);
		imshow("Final", img);
		waitKey(1);
	}
	return 0;
}
