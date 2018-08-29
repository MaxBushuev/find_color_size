#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using std::vector;
int main()
{
	Mat img;
	Mat hsv;
	Mat findColor;

	int findHMin=0;
	int findSMin=0;
	int findVMin=0;
	int findHMax=0;
	int findSMax=0;
	int findVMax=0;

	int lengthMin=0;
	int lengthMax=0;

	int ratioMin = 50;
	int ratioMax = 50;

	float ratioMinD;
	float ratioMaxD;
	VideoCapture cap(0);
	
	while (true)
	{

		cap >> img;
		cvtColor(img, hsv, COLOR_RGB2HSV);
		createTrackbar("Hmin", "Color", &findHMin, 180);
		createTrackbar("Smin", "Color", &findSMin, 255);
		createTrackbar("Vmin", "Color", &findVMin, 255);
		createTrackbar("Hmax", "Color", &findHMax, 180);
		createTrackbar("Smax", "Color", &findSMax, 255);
		createTrackbar("Vmax", "Color", &findVMax, 255);

		createTrackbar("LengthMin", "Size", &lengthMin, 1000);
		createTrackbar("RatioMin", "Size", &ratioMin, 180);
		createTrackbar("LenghtMax", "Size", &lengthMax, 1000);
		createTrackbar("RatioMax", "Size", &ratioMax, 180);
		ratioMinD = ratioMin / 100;
		ratioMaxD = ratioMax / 100;
		inRange(hsv, Scalar(findHMin, findSMin, findVRed), Scalar(findHMax, findSMax, findVMax), findColor);
		imshow("Color", findColor);
		vector<vector<Point>> contours;
		
		findContours(findColor, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0,0));
		vector<vector<Point> > contours_poly(contours.size() + 1);
		vector<Rect> boundRect(contours.size());
		for (size_t i = 0; i < contours.size(); i++)
		{
			approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));

		}
		for (size_t i = 0; i < contours.size(); i++)
		{
			if (boundRect[i].height > lengthMin && boundRect[i].width > lengthMin&& boundRect[i].height < lengthMax && boundRect[i].width < lengthMax && ratioMinD < boundRect[i].height / boundRect[i].width < ratioMaxD)
			{
				drawContours(findColor, contours_poly, (int)i, Scalar(0, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
				rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2, 8, 0);
			}
		}
		imshow("Size", findColor);
		imshow("Final", img);
		waitKey(1);
	}
	return 0;
}
