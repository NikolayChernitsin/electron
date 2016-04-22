#include "elemwidget.h"
#include <QSplitter>
#include <QLayout>
#include <QMenu>
#include <QWidget>

ElemWidget::ElemWidget(QWidget *parent) : QWidget(parent)
{
    RotLeft = new QPushButton("RotL");
    RotRight = new QPushButton("RotR");
    RefX = new QPushButton("RefX");
    RefY = new QPushButton("RefY");

    graphicsView = new QGraphicsView;

    QVBoxLayout *BtmLayout = new QVBoxLayout;
    BtmLayout->setMargin(0);
    BtmLayout->addWidget(RotLeft);
    BtmLayout->addWidget(RotRight);
    BtmLayout->addWidget(RefX);
    BtmLayout->addWidget(RefY);

    QHBoxLayout *ElemLayout = new QHBoxLayout;
    ElemLayout->setMargin(0);
    ElemLayout->addLayout(BtmLayout);
    ElemLayout->addWidget(graphicsView);

    setLayout(ElemLayout);
/*
    QWidget *ElWidg = new QWidget;
    ElWidg ->setLayout(ElemLayout);
    setCentralWidget(ElWidg);*/
}
