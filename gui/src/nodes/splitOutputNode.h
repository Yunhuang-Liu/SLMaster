#ifndef __SPLIT_OUTPUT_NODE_H_
#define __SPLIT_OUTPUT_NODE_H_

#include "QuickQanava.h"

#include "flowNode.h"
#include "typeDef.h"

class SplitOutputNode : public FlowNode {
    Q_OBJECT
    Q_PROPERTY_AUTO(int, portIndex)
  public:
    SplitOutputNode() : portIndex_(0) {};
    static  QQmlComponent*      delegate(QQmlEngine& engine) noexcept;
  protected slots:
    void inNodeOutputChanged();
};

QML_DECLARE_TYPE(SplitOutputNode)

#endif // !__SPLIT_OUTPUT_NODE_H_
