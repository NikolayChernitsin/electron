#ifndef ELECTRON_H
#define ELECTRON_H
#include "electronelement.h"
#include "libs/containers/tree.h"
#include "components/electronscheme.h"

typedef ElectronElement ElectronItem;
typedef ElectronItem::Axis ElectronAxis;
typedef Tree<ElectronItem> ElectronTree;

class Electron
{
public:
    Electron();
    virtual ~Electron();

    inline const ElectronTree *tree() const { return &m_tree; }

    bool setCurrent(const ElectronItem &item);
    inline ElectronItem *current() const { return m_current; }

    inline ElectronScheme *scheme() const { return m_scheme; }

    void rotateCurrent(float angle);
    void reflectCurrent(ElectronAxis axis);

    inline void newScheme() { m_scheme->newScheme(); }
    inline void saveToFile(const std::string &filename) { m_scheme->saveToFile(filename); }
    inline bool loadFromFile(const std::string &filename) { return m_scheme->loadFromFile(filename); }

private:
    bool xmlParse();

    ElectronItem *m_current;
    ElectronTree m_tree;
    ElectronScheme *m_scheme;
};

#endif // ELECTRON_H
