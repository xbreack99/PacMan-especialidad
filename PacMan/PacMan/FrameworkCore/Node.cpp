#include "Node.h"

Node::Node():id(1)
{

}

Node::~Node()
{
}

void Node::AddComponent(IComponent* component)
{
    mComponents[component->GetType()] = component;
}