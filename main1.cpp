#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include "PatternTemplates 1.h"

using namespace std;


enum class ComponentWeight : int
{
    Heavy,
    Normal,
    LightWeight,
    Unknown
};
enum class PCManagerEnum : int
{
  Insert,
  Break,
  PullOut,
  Sell,
  None
};
class PCStrategy
{
public:
  virtual ~PCStrategy() {}
  virtual void displayDetails() = 0;
};

class InsertStrategy : public PCStrategy
{
  void displayDetails() { cout << "Insert component into PC..."; }
};

class BreakStrategy : public PCStrategy
{
  void displayDetails() { cout << "Break component..."; }
};

class PullOutStrategy : public PCStrategy
{
  void displayDetails() { cout << "Pull out component into PC..."; }
};
class SellStrategy : public PCStrategy
{
  void displayDetails() { cout << "Sell using avito..."; }
};
PCStrategy *CreatePCStrategy(PCManagerEnum pcManner)
{
  switch(pcManner)
  {
    case PCManagerEnum::Insert: return new InsertStrategy;
    case PCManagerEnum::Break: return new BreakStrategy;
    case PCManagerEnum::PullOut: return new PullOutStrategy;
    case PCManagerEnum::Sell: return new SellStrategy;


    default: return nullptr;
  }
}

class Component {
private:
    ComponentWeight Weight;
    string Name;
    string Manufacturer;
    int Price;
    PCStrategy *PCManner;

void DoPCUsingStrategy()
{
  if(PCManner == nullptr)
  {
    
    cout << "Do nothing!";
    return;
  }
  else
  {
    
    PCManner->displayDetails();
  }
}

void DetectGoodOrNot()
{
  if(IsGood())
  {
      cout << "GOOD";
  }
  else
  {
      cout << "BAD";
  }
}
protected:
    bool ComponentIsGood;
public:
    Component(ComponentWeight weight) : Weight(weight), Manufacturer(""), Name(""),ComponentIsGood(false),Price(0), PCManner(nullptr)
    {

        ComponentIsGood = static_cast<bool>(rand()%2);
    }

    virtual ~Component() {
    if(PCManner != nullptr) delete PCManner;
    }

   
    bool IsGood() const { return ComponentIsGood; }

   
    ComponentWeight GetWeight() const { return Weight; }

    
    int getPrice() const {return Price;}
    virtual void PrintType() = 0;



    void displayDetails()
    {
        
        PrintType();
        cout << " : ";
        DetectGoodOrNot();
        cout << " : ";
        DoPCUsingStrategy();  
        cout << endl;
}


void SetPCManner(PCStrategy *pcManner) { PCManner = pcManner; }
};

class CPU : public Component
{
public:
    CPU();
    ~CPU() {}

    void PrintType() { cout << "CPU"; }
};


CPU::CPU() : Component(ComponentWeight::LightWeight)
{
    SetPCManner(CreatePCStrategy(PCManagerEnum::Insert));
}


class GPU : public Component
{
public:
    GPU() : Component(ComponentWeight::Heavy){SetPCManner(CreatePCStrategy(PCManagerEnum::Break));}
    ~GPU() {}


    void PrintType() { cout << "GPU"; }
};



class RAM : public Component
{
public:
    RAM() : Component(ComponentWeight::Normal){SetPCManner(CreatePCStrategy(PCManagerEnum::PullOut));}
    ~RAM() {}

    void PrintType() { cout << "RAM"; }
};




enum class ComponentType : int
{
    CPU = 1,
    RAM = 2,
    GPU = 3,

    Undefined = 0
};

Component *CreateComponent(ComponentType type)
{
    Component *newComponent = nullptr;

    if(type == ComponentType::CPU)
    {
        newComponent = new CPU;
    }
    else if(type == ComponentType::RAM)
    {
        newComponent = new RAM;
    }
    else if(type == ComponentType::GPU)
    {
        newComponent = new GPU;
    }

    return newComponent;
}




class ComponentWeightDecorator : public IteratorDecorator<class Component*>
{
private:
    ComponentWeight TargetWeight;

public:
    ComponentWeightDecorator(Iterator<Component*> *it, ComponentWeight weight)
    : IteratorDecorator<Component*>(it), TargetWeight(weight) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->GetWeight() != TargetWeight)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->GetWeight() != TargetWeight);
    }
};



class ComponentGoodDecorator : public IteratorDecorator<class Component*>
{
private:
    bool TargetGood;

public:
    ComponentGoodDecorator(Iterator<Component*> *it, bool isGood)
    : IteratorDecorator<Component*>(it), TargetGood(isGood) {}

    void First()
    {
        It->First();
        while(!It->IsDone() && It->GetCurrent()->IsGood() != TargetGood)
        {
            It->Next();
        }
    }

    void Next()
    {
        do
        {
            It->Next();

        } while(!It->IsDone() && It->GetCurrent()->IsGood() != TargetGood);
    }
};


void InsertAll(Iterator<Component*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Component *currentComponent = it->GetCurrent();
        currentComponent->displayDetails();
    }
}


void InsertAllGood(Iterator<Component*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Component *currentComponent = it->GetCurrent();
        if(!currentComponent->IsGood()) continue;

        currentComponent->displayDetails();
    }
}


void InsertAllLightWeight(Iterator<Component*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Component *currentComponent = it->GetCurrent();
        if(currentComponent->GetWeight() != ComponentWeight::LightWeight) continue;

        currentComponent->displayDetails();
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    size_t N = 30;

    

    ArrayClass<Component*> ComponentArray;
    for(size_t i=0; i<N; i++)
    {
        int Component_num = rand()%3+1; 
        ComponentType Component_type = static_cast<ComponentType>(Component_num);
        Component *newComponent = CreateComponent(Component_type);
        newComponent->SetPCManner(CreatePCStrategy(PCManagerEnum::Sell));
        ComponentArray.Add(newComponent);
    }

    wcout << L"Размер массива комплектующих: " << ComponentArray.Size() << endl;

   

    list<Component*> ComponentVector; 
    for(size_t i=0; i<N; i++)
    {
        int Component_num = rand()%3+1;
        ComponentType Component_type = static_cast<ComponentType>(Component_num);
        Component *newComponent = CreateComponent(Component_type);
        ComponentVector.push_back(newComponent); 
    }

    wcout << L"Размер списка комплектующих: " << ComponentVector.size() << endl;

    
    cout << endl << "Insert all in a simple loop:" << endl;
    for(size_t i=0; i<ComponentArray.Size(); i++)
    {
        Component *currentComponent = ComponentArray[i];
        currentComponent->displayDetails();
    }

    
    cout << endl << "Insert all using iterator:" << endl;
    Iterator<Component*> *allIt = ComponentArray.GetIterator();
    InsertAll(allIt);
    delete allIt;

    
    cout << endl << "Inset all good using iterator:" << endl;
    Iterator<Component*> *goodIt = new ComponentGoodDecorator(ComponentArray.GetIterator(), true);
    InsertAll(goodIt);
    delete goodIt;

    
    cout << endl << "Inset all Normal using iterator:" << endl;
    Iterator<Component*> *normalIt = new ComponentWeightDecorator(ComponentArray.GetIterator(), ComponentWeight::Normal);
    InsertAll(normalIt);
    delete normalIt;

    
    cout << endl << "Inset all good normal using iterator:" << endl;
    Iterator<Component*> *goodNormalIt =
        new ComponentGoodDecorator(new ComponentWeightDecorator(ComponentArray.GetIterator(), ComponentWeight::Normal), true);
    InsertAll(goodNormalIt);
    delete goodNormalIt;

    
    cout << endl << "Insert all good normal using adapted iterator (another container):" << endl;
    Iterator<Component*> *adaptedIt = new ConstIteratorAdapter<std::list<Component*>, Component*>(&ComponentVector);
    Iterator<Component*> *adaptedNormalIt = new ComponentGoodDecorator(new ComponentWeightDecorator(adaptedIt, ComponentWeight::Normal), true);
    InsertAll(adaptedNormalIt);
    delete adaptedNormalIt;
    

    return 0;
}


