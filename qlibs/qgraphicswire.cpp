#include "qgraphicswire.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <iostream>

class QGraphicsWirePrivate : public QGraphicsObject
{
public:
    QGraphicsWirePrivate(QGraphicsWire *parent = 0);
    QGraphicsWirePrivate(QGraphicsWire *parent, const QGraphicsWirePrivate &wire);

    void setSp(const QPointF &point);
    void setEp(const QPointF &point);

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void setComplete(bool st = true);

private:
    QPointF m_sp, m_ep;
    bool m_complete;
    QGraphicsWire *m_parent;

    friend class QGraphicsWire;
};

QGraphicsWirePrivate::QGraphicsWirePrivate(QGraphicsWire *parent) :
    QGraphicsObject(parent), m_complete(parent->m_type == QGraphicsWire::Model ? true : false), m_parent(parent)
{
}

QGraphicsWirePrivate::QGraphicsWirePrivate(QGraphicsWire *parent, const QGraphicsWirePrivate &wire) :
    QGraphicsObject(parent), m_sp(wire.m_sp), m_ep(wire.m_ep), m_complete(wire.m_complete), m_parent(parent)
{
}

void QGraphicsWirePrivate::setSp(const QPointF &point)
{
    m_sp = mapFromScene(point);
    update();
}

void QGraphicsWirePrivate::setEp(const QPointF &point)
{
    m_ep = mapFromScene(point);
    update();
}

QRectF QGraphicsWirePrivate::boundingRect() const
{
    return QRectF(std::min(m_sp.x(), m_ep.x()) - 1.5, std::min(m_sp.y(), m_ep.y()) - 1.5, std::abs(m_sp.x() - m_ep.x()) + 3, std::abs(m_sp.y() - m_ep.y()) + 3);
}

QPainterPath QGraphicsWirePrivate::shape() const
{
    QPainterPath path;
    if(m_complete)
        path.addRect(boundingRect());
    return path;
}

void QGraphicsWirePrivate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(m_parent->color);
    painter->drawLine(m_sp, m_ep);
}

void QGraphicsWirePrivate::setComplete(bool st)
{
    m_complete = st;
    update();
}

/*****************************************************************
 *****************************************************************
 *****************************************************************/

QGraphicsWire::QGraphicsWire() :
    QGraphicsObject(NULL), color(Qt::black), m_from(NULL), m_to(NULL), m_ewire(new ElectronWire), m_type(User)
{
    setAcceptHoverEvents(true);
    setScale(2.0);
    QAction *a_del = m_menu.addAction(QString::fromUtf8("Удалить"));

    connect(a_del, SIGNAL(triggered()), SLOT(userDelete()));
}

QGraphicsWire::QGraphicsWire(const QGraphicsWire &wire) :
    QGraphicsObject(NULL)
{
    setAcceptHoverEvents(true);
    setScale(wire.scale());
    setPos(wire.pos());
    for(int i = 0; i < wire.m_items.size(); i++)
        m_items.push_back(new QGraphicsWirePrivate(this, *wire.m_items[i]));
    m_from = wire.m_from;
    m_to = wire.m_to;
    color = wire.color;
    QAction *a_del = m_menu.addAction(QString::fromUtf8("Удалить"));
    m_ewire = wire.m_ewire;
    m_type = wire.m_type;

    connect(a_del, SIGNAL(triggered()), SLOT(userDelete()));
}

QGraphicsWire::QGraphicsWire(ElectronWire *wire) :
    QGraphicsObject(NULL), color(Qt::black), m_from(NULL), m_to(NULL), m_ewire(wire), m_type(Model)
{
    setAcceptHoverEvents(true);
    setScale(2.0);

    QAction *a_del = m_menu.addAction(QString::fromUtf8("Удалить"));

    connect(a_del, SIGNAL(triggered()), SLOT(userDelete()));

    setPos(m_ewire->pos());

    for(ElectronWire::iterator iter = m_ewire->begin() + 1; iter != m_ewire->end(); iter++) {
        //addPoint(QPointF((*iter).x(), (*iter).y()));
        QPointF point((*iter).x(), -(*iter).y());
        QGraphicsWirePrivate *wire = new QGraphicsWirePrivate(this);
        if(m_items.empty()) {
            wire->m_sp = QPointF(0, 0);
        } else {
            wire->m_sp = m_items.last()->m_ep;
        }
        wire->m_ep = point;
        m_items.push_back(wire);
    }
}

QGraphicsWire &QGraphicsWire::operator =(const QGraphicsWire &wire)
{
    setAcceptHoverEvents(true);
    setScale(wire.scale());
    setPos(wire.pos());
    for(int i = 0; i < wire.m_items.size(); i++)
        m_items.push_back(new QGraphicsWirePrivate(this, *wire.m_items[i]));
    m_from = wire.m_from;
    m_to = wire.m_to;
    color = wire.color;
    QAction *a_del = m_menu.addAction(QString::fromUtf8("Удалить"));
    m_ewire = wire.m_ewire;
    m_type = wire.m_type;

    connect(a_del, SIGNAL(triggered()), SLOT(userDelete()));
    return *this;
}

void QGraphicsWire::setFromItem(QGraphicsChip *item)
{
    m_from = item;
}

void QGraphicsWire::setToItem(QGraphicsChip *item)
{
    m_to = item;
}

void QGraphicsWire::addPoint(const QPointF &point)
{
    QGraphicsWirePrivate *wire = new QGraphicsWirePrivate(this);
    //wire->setScale(2.0);
    if(m_items.empty()) {
        wire->m_sp = QPointF(0, 0);
    } else {
        wire->m_sp = m_items.last()->m_ep;
    }
    //wire->setPos(pos());
    wire->setEp(point);
    //m_scene->addItem(wire);
    m_items.push_back(wire);
    update();
}

void QGraphicsWire::replaceLastPoint(const QPointF &point)
{
    if(!m_items.empty()) {
        QGraphicsWirePrivate *wire = m_items.last();
        wire->setEp(point);
    }
    scene()->update();
}

bool QGraphicsWire::deleteLastPoint()
{
    if(!m_items.empty()) {
        //m_scene->removeItem(m_items.last());
        delete m_items.last();
        m_items.removeLast();
    }
    scene()->update();
    return m_items.empty();
}

bool QGraphicsWire::setComplete(bool st)
{
    deleteLastPoint();
    if(m_items.isEmpty())
        return false;
    for(QGraphicsWirePrivate* wire : m_items) {
        if(m_type == User) {
            Basic::Point bp(wire->m_sp.x(), wire->m_sp.y());
            m_ewire->addPoint(bp);
        }
        wire->setComplete(st);
    }
    if(m_type == User) {
        QGraphicsWirePrivate* wire = m_items.last();
        Basic::Point bp(wire->m_ep.x(), wire->m_ep.y());
        m_ewire->addPoint(bp);
        m_ewire->setPos(Basic::Point(pos().x(), pos().y()));
    }
    scene()->update();
    return true;
}

QRectF QGraphicsWire::boundingRect() const
{
    QRectF rect;
    for(QGraphicsWirePrivate* wire : m_items) {
        QRectF br = wire->boundingRect();
        rect.setLeft(std::min(rect.left(), br.left()));
        rect.setRight(std::max(rect.right(), br.right()));
        rect.setTop(std::min(rect.top(), br.top()));
        rect.setBottom(std::max(rect.bottom(), br.bottom()));
    }
    return rect;
}

QPainterPath QGraphicsWire::shape() const
{
    QPainterPath path;
    for(QGraphicsWirePrivate* wire : m_items) {
        //path.addRect(wire->boundingRect());
        path.addPath(wire->shape());
    }
    return path;
}

void QGraphicsWire::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    //painter->drawPath(shape());
    //painter->drawRect(boundingRect());
}

void QGraphicsWire::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    color = Qt::red;
    update();
}

void QGraphicsWire::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    color = Qt::black;
    update();
}

void QGraphicsWire::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    m_menu.popup(event->screenPos());
}

void QGraphicsWire::userDelete()
{
    emit deleteMe();
}

int QGraphicsWire::type() const
{
    return QWire;
}

void QGraphicsWire::setPos(const Basic::Point &point)
{
    setPos(point.x(), -point.y());
}

void QGraphicsWire::setPos(const QPointF &pos)
{
    QGraphicsObject::setPos(pos);
}

void QGraphicsWire::setPos(qreal x, qreal y)
{
    setPos(QPointF(x, y));
}
