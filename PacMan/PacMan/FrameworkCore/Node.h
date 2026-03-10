#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <cstdint>
#include <iterator>
#include <memory>
#include "IComponent.h"

using std::map;


//this will be like gameobject
class Node
{
    
 public:
    Node();

    ~Node();

   // Non-template overload for IComponent* to guarantee linkage
   void AddComponent(IComponent* component);

   template <typename T>
   T* GetComponent();

 public:

   // map of components keyed by their type
   std::map<ComponentType, IComponent*> mComponents;
   
 private:
    uint32_t id;
};

template <typename T>
T* Node::GetComponent()
{ 
    for (auto& pair : mComponents)
    {
        IComponent* component = pair.second;
        if (T* comp = dynamic_cast<T*>(component))
        {
            return comp;
        }
    }
    return nullptr;
}