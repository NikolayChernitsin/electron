#include "elemwidget.h"
#include <QSplitter>
#include <QLayout>
#include <QMenu>
#include <QWidget>
#include <QToolButton>
#include "qlibs/qgraphicschip.h"
#include <iostream>

ElemWidget::ElemWidget(QWidget *parent) :
    QWidget(parent), m_model(0)
{
    RotLeft = new QPushButton;
    RotLeft->setIcon(QIcon(":/left.png"));
    RotLeft->setIconSize(QSize(35,35));

    RotRight = new QPushButton;
    RotRight->setIcon(QIcon(":/right.png"));
    RotRight->setIconSize(QSize(35,35));

    RefX = new QPushButton;
    RefX->setIcon(QIcon(":/side.png"));
    RefX->setIconSize(QSize(35,35));

    RefY = new QPushButton;
    RefY->setIcon(QIcon(":/updown.png"));
    RefY->setIconSize(QSize(35,35));

    RotLeft->setEnabled(false);
    RotRight->setEnabled(false);
    RefX->setEnabled(false);
    RefY->setEnabled(false);

    graphicsView = new QGraphicsView;

    scene = new QGraphicsScene;

    graphicsView->setScene(scene);
    //rectangle ->setFlag(QGraphicsItem::ItemIsMovable);

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
}

void ElemWidget::setModel(QPreviewModel *model)
{
    m_model = model;
    connect(RotLeft, SIGNAL(pressed()), m_model, SLOT(rotateLeft()));
    connect(RotRight, SIGNAL(pressed()), m_model, SLOT(rotateRight()));
    connect(RefX, SIGNAL(pressed()), m_model, SLOT(reflectX()));
    connect(RefY, SIGNAL(pressed()), m_model, SLOT(reflectY()));
    connect(m_model, SIGNAL(currentUpdated()), SLOT(curentChanged()));
}

void ElemWidget::curentChanged()
{
    foreach(QGraphicsItem *item, scene->items()) {
        delete item;
    }
    scene->items().clear();
    scene->update();
    QGraphicsItem *chip = new QGraphicsChip(m_model->current());
    scene->addItem(chip);
    chip->setScale(2);
}

