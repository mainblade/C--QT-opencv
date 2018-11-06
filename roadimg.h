#ifndef ROADIMG_H
#define ROADIMG_H

#include <QtGui/QMainWindow>
#include "ui_roadimg.h"

class roadimg : public QMainWindow
{
	Q_OBJECT

public:
	roadimg(QWidget *parent = 0, Qt::WFlags flags = 0);
	~roadimg();

 	private slots:  //slots to signal
 		void openImage();


private:
	Ui::roadimgClass ui;
};

#endif // ROADIMG_H
