#include "treenode.h"

bool TreeNode::isFarEnough(qreal distance) {
    return getWidth() / distance <= tolerance;
}
