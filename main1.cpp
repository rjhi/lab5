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

class Component {
private:
    ComponentWeight Weight;
    string Name;
    string Manufacturer;
    int Price;
protected:
    bool ComponentIsGood;
public:
    Component(ComponentWeight weight) : Weight(weight), Manufacturer(""), Name(""),ComponentIsGood(false),Price(0)
    {
        // Значение инициализируется случайным числом 0 или 1
        ComponentIsGood = static_cast<bool>(rand()%2);
    }

    virtual ~Component() {} // Деструктор (объявлен виртуальным, чтобы обеспечить корректное уничтожение унаследованных классов)

    // Функция с реализацией
    bool IsGood() const { return ComponentIsGood; }

    // Функция с реализацией
    ComponentWeight GetWeight() const { return Weight; }

    // Функция с реализацией
    int getPrice() const {return Price;}
    // Функция с реализацией
    virtual void displayDetails()
    {
        if(IsGood())
        {
            cout << "Integration GOOD component... ";
        }
        else
        {
            cout << "Integration BAD component... ";
        }
    }
};

class CPU : public Component
{
public:
    CPU();
    ~CPU() {}

    void displayDetails() override;
};

// Реализация конструктора
CPU::CPU() : Component(ComponentWeight::LightWeight)
{
}

void CPU::displayDetails()
{
    Component::displayDetails(); // Вызов функции, определенной в базовом классе
    cout << "integration CPU into a computer" << endl;
}

class GPU : public Component
{
public:
    GPU() : Component(ComponentWeight::Heavy){}
    ~GPU() {}


    void displayDetails()override;
};

void GPU::displayDetails()
{
    cout << "integration GPU into a computer" << endl;
}


class RAM : public Component
{
public:
    RAM() : Component(ComponentWeight::Normal){}
    ~RAM() {}

   void displayDetails()override;
};

void RAM::displayDetails()
{
    cout << "integration RAM into a computer" << endl;
}


// Реализация паттерна "Фабричный метод" для создания фруктов

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


// Декоратор итератора для выделения фруктов по цвету

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

// Декоратор итератора для выделения "хороших" или "плохих" фруктов

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

// Функция, позволяющая "съесть" любые фрукты из любого контейнера
// вне зависимости от его внутреннего устройства
void InsertAll(Iterator<Component*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Component *currentComponent = it->GetCurrent();
        currentComponent->displayDetails();
    }
}

// Функция, позволяющая "съесть" только хорошие фрукты
// (демонстрация решения проблемы "в лоб")
void InsertAllGood(Iterator<Component*> *it)
{
    for(it->First(); !it->IsDone(); it->Next())
    {
        Component *currentComponent = it->GetCurrent();
        if(!currentComponent->IsGood()) continue;

        currentComponent->displayDetails();
    }
}

// Функция, позволяющая "съесть" только зеленые фрукты
// (демонстрация решения проблемы "в лоб")
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

    // Массив фруктов

    ArrayClass<Component*> ComponentArray;
    for(size_t i=0; i<N; i++)
    {
        int Component_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        ComponentType Component_type = static_cast<ComponentType>(Component_num);
        Component *newComponent = CreateComponent(Component_type);
        ComponentArray.Add(newComponent);
    }

    wcout << L"Размер массива комплектующих: " << ComponentArray.Size() << endl;

    // Связанный список фруктов (для демонстрации адаптера)

    list<Component*> ComponentVector; // а еще можно vector, forward_list, ...
    for(size_t i=0; i<N; i++)
    {
        int Component_num = rand()%3+1; // Число от 1 до 3 (случайный фрукт)
        ComponentType Component_type = static_cast<ComponentType>(Component_num);
        Component *newComponent = CreateComponent(Component_type);
        ComponentVector.push_back(newComponent); // Добавить новый фрукт в конец списка
    }

    wcout << L"Размер списка комплектующих: " << ComponentVector.size() << endl;

    // Обход в простом цикле
    cout << endl << "Insert all in a simple loop:" << endl;
    for(size_t i=0; i<ComponentArray.Size(); i++)
    {
        Component *currentComponent = ComponentArray[i];
        currentComponent->displayDetails();
    }

    // Обход всех элементов при помощи итератора
    cout << endl << "Insert all using iterator:" << endl;
    Iterator<Component*> *allIt = ComponentArray.GetIterator();
    InsertAll(allIt);
    delete allIt;

    // Обход всех хороших фруктов
    cout << endl << "Inset all good using iterator:" << endl;
    Iterator<Component*> *goodIt = new ComponentGoodDecorator(ComponentArray.GetIterator(), true);
    InsertAll(goodIt);
    delete goodIt;

    // Обход всех оранжевых фруктов
    cout << endl << "Inset all Normal using iterator:" << endl;
    Iterator<Component*> *normalIt = new ComponentWeightDecorator(ComponentArray.GetIterator(), ComponentWeight::Normal);
    InsertAll(normalIt);
    delete normalIt;

    // Обход всех хороших оранжевых фруктов
    cout << endl << "Inset all good normal using iterator:" << endl;
    Iterator<Component*> *goodNormalIt =
        new ComponentGoodDecorator(new ComponentWeightDecorator(ComponentArray.GetIterator(), ComponentWeight::Normal), true);
    InsertAll(goodNormalIt);
    delete goodNormalIt;

    // Демонстрация работы адаптера
    cout << endl << "Insert all good normal using adapted iterator (another container):" << endl;
    Iterator<Component*> *adaptedIt = new ConstIteratorAdapter<std::list<Component*>, Component*>(&ComponentVector);
    Iterator<Component*> *adaptedNormalIt = new ComponentGoodDecorator(new ComponentWeightDecorator(adaptedIt, ComponentWeight::Normal), true);
    InsertAll(adaptedNormalIt);
    delete adaptedNormalIt;
    // adaptedIt удалять не надо, так как он удаляется внутри декоратора adaptedOrangeIt

    return 0;
}

