#include "roadimg.h" //your project's head name
#include "ui_roadimg.h" //your project's about qt designer name

#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QDebug>

#include <iostream>
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <QMessageBox>

using namespace std;
using namespace cv;

roadimg::roadimg(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	connect(ui.btn_openimg, SIGNAL(clicked()),this, SLOT(openImage()));
}

roadimg::~roadimg()
{

}

int OTSU(const Mat srcImage);

void roadimg::openImage()
{
	QString pFileName;
	pFileName=QFileDialog::getOpenFileName(0,QString::fromLocal8Bit("Select IMG："),QString::fromLocal8Bit("C:\\Users\\zj790\\Desktop\\"),/*".",*/QString::fromLocal8Bit("Images (*.png *.bmp *.jpg *.tif *.GIF )"));

		Mat srcImage,grayImage,dstImage;
		char*  pChar;

		QByteArray pByteArray = pFileName.toLatin1();    

		pChar=pByteArray.data();
		srcImage=imread(pChar,1);
		cvtColor(srcImage,grayImage,CV_RGB2GRAY);

		int thresholdValue=OTSU(grayImage);

		qDebug()<<thresholdValue;

		threshold(grayImage,dstImage,thresholdValue,255,THRESH_BINARY);
		
		QImage pImage;
		pImage=QImage((const unsigned char*)dstImage.data,dstImage.cols,dstImage.rows,QImage::Format_Indexed8);
		
		pImage = pImage.scaled(ui.lb_displayimg->size(),
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation);
		ui.lb_displayimg->setPixmap(QPixmap::fromImage(pImage));
}

int OTSU(Mat srcImage) 
{
	int nCols = srcImage.cols;
	int nRows = srcImage.rows;
	int threShold = 0;
	//init the parameters
	int nSumPix[256];
	float nProDis[256];
	for (int i = 0; i < 256; i++)
	{
		nSumPix[i] = 0;
		nProDis[i] = 0;
	}

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			nSumPix[(int)srcImage.at<uchar>(i, j)]++;
		}
	}

	for (int i = 0; i < 256; i++)
	{
		nProDis[i] = (float)nSumPix[i] / (nCols*nRows);
	}

	float w0, w1, u0_temp, u1_temp, u0, u1, delta_temp;
	double delta_max = 0.0;
	for (int i = 0; i < 256; i++)
	{
		w0 = w1 = u0 = u1 = u0_temp = u1_temp = delta_temp = 0;
		for (int j = 0; j < 256; j++)
		{

			if (j <= i)
			{
				w0 += nProDis[j];
				u0_temp += j*nProDis[j];
			}

			else
			{
				w1 += nProDis[j];
				u1_temp += j*nProDis[j];
			}
		}

		u0 = u0_temp / w0;
		u1 = u1_temp / w1;
		delta_temp = (float)(w0*w1*pow((u0 - u1), 2)); 
		if (delta_temp > delta_max)
		{
			delta_max = delta_temp;
			threShold = i;
		}
	}
	return threShold;
}
