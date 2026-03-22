#include "Node.h"

Node::Node():id(1)
{

}

Node::~Node()
{
    mComponents.clear();
}

void Node::AddComponent(IComponent* component)
{
    mComponents[component->GetType()] = component;
}