#include <iostream>

using namespace std;

class Part {
    int itsPartNumber;
public:
    Part();
    Part(int purtNumber);
    virtual ~Part();
    int getItsPartNumber() const;
    virtual void Display() const = 0;
};

class CarPart : public Part {
    int itsModelYer;
public:
    CarPart();
    CarPart(int yer, int purtNumber);
    ~CarPart() override;
    void Display() const override;
};

class AirPlanePart : public Part {
    int itsEngineNumber;
public:
    AirPlanePart();
    AirPlanePart(int engineNumber, int partNumber);
    ~AirPlanePart() override;
    virtual void Display() const override;
};

//************Узлы списка деталей***************

class PartNode {
    Part *itsPart;
    PartNode *itsNext;
public:
    PartNode(Part *pPart);
    ~PartNode();

    PartNode *getItsNext() const;
    void setItsNext(PartNode *value);
    Part *getItsPart() const;
};

class PartList {
    PartNode *pHead;
    int itsCount;
    static PartList GlobalPartsList;
public:
    PartList();
    ~PartList();

    Part* Find(int &position, int PartNumber) const;
    int getItsCount() const;
    Part* getFirst() const;
    static PartList getGlobalPartsList();
    void insert(Part *volume);
    void iterate(void(Part::*func)()const) const;
    Part* operator[](int offSet) const;
};

PartList PartList::GlobalPartsList;

int main() {
    PartList pl = PartList::getGlobalPartsList();
    Part * pPart = nullptr;
    int partNumber;
    int value;
    int choice;

    while (1)
    {
        cout <<"(0)Quit (1)Car (2)Plane: ";
        cin >> choice;
        if (!choice)
            break;

        cout << "New PartNumber?: ";
        cin >> partNumber;

        if (choice == 1)
        {
            cout << "Model Yer?: ";
            cin >> value;
            pPart = new CarPart(value, partNumber);
        }
        else
        {
            cout << "Engine Number?: ";
            cin >> value;
            pPart = new AirPlanePart(value, partNumber);
        }

        pl.insert(pPart);
    }
    void (Part::*pFunc)()const = &Part::Display;
    pl.iterate(pFunc);
    return 0;
}

//Реализация класса part
int Part::getItsPartNumber() const
{
    return itsPartNumber;
}

void Part::Display() const
{
    cout << "Detail number: " << itsPartNumber << endl;
}

Part::Part():itsPartNumber(1) {}

Part::Part(int purtNumber):itsPartNumber(purtNumber) {}

Part::~Part()
{

}

//реализация класса CarPart
CarPart::CarPart():itsModelYer(94) {}

CarPart::CarPart(int yer, int purtNumber):Part(purtNumber), itsModelYer(yer) {}

CarPart::~CarPart()
{

}

void CarPart::Display() const
{
    Part::Display();
    cout << "Year of creation: " << itsModelYer << endl;
}

//рефлизация класса AirPlanePart
AirPlanePart::AirPlanePart() : itsEngineNumber(1) {}

AirPlanePart::AirPlanePart(int engineNumber, int partNumber) : Part(partNumber), itsEngineNumber(engineNumber) {}

AirPlanePart::~AirPlanePart()
{

}

void AirPlanePart::Display() const
{
    Part::Display();
    cout << "Engine number: " << itsEngineNumber << endl;
}

//*************Реализация узлов списка деталей***********
//реализация PartNode
Part *PartNode::getItsPart() const
{
    if (itsPart)
        return itsPart;
    else
        return nullptr;
}

PartNode::PartNode(Part *pPart) : itsPart(pPart), itsNext(nullptr) {}

PartNode::~PartNode()
{
    delete itsPart;
    itsPart = nullptr;
    delete itsNext;
    itsNext = nullptr;
}

PartNode *PartNode::getItsNext() const
{
return itsNext;
}

void PartNode::setItsNext(PartNode *value)
{
itsNext = value;
}

//Реализация PartList
PartList PartList::getGlobalPartsList()
{
    return GlobalPartsList;
}

void PartList::insert(Part *volume)
{
    PartNode *pNode = new PartNode(volume);
    PartNode *pCurrent = pHead;
    PartNode *pNext = nullptr;
    int New = volume->getItsPartNumber();
    int Next = 0;
    itsCount++;

    if(!pHead)
    {
        pHead = pNode;
        return;
    }

    if (pHead->getItsPart()->getItsPartNumber()>New)
    {
        pNode->setItsNext(pHead);
        pHead = pNode;
        return;
    }

    for (;;)
    {
        if (!pCurrent->getItsNext())
        {
            pCurrent->setItsNext(pNode);
            return;
        }

        pNext = pCurrent->getItsNext();
        Next = pNext->getItsPart()->getItsPartNumber();
        if (Next>New)
        {
            pCurrent->setItsNext(pNode);
            pNode->setItsNext(pNext);
            return;
        }

        pCurrent = pNext;
    }
}

void PartList::iterate(void (Part::*func)() const) const
{
    if(!pHead)
        return;
    PartNode *pNode = pHead;
    do
        (pNode->getItsPart()->*func)();
    while ((pNode = pNode->getItsNext()));
}

Part *PartList::operator[](int offSet) const
{
    PartNode *pNode = pHead;

    if (!pHead)
        return nullptr;
    if (offSet>itsCount)
        return nullptr;
    for (int i=0; i<offSet; i++)
        pNode = pNode->getItsNext();

    return pNode->getItsPart();
}

PartList::PartList():pHead(nullptr),itsCount(0) {}

PartList::~PartList()
{
    delete pHead;
}

Part *PartList::Find(int &position, int PartNumber) const
{
    PartNode *pNode = nullptr;
    for (pNode = pHead, position = 0; pNode != nullptr; pNode = pNode->getItsNext(), position++)
    {
        if (pNode->getItsPart()->getItsPartNumber() == PartNumber)
            break;
    }
    if (pNode == nullptr)
        return nullptr;
    else
        return pNode->getItsPart();
}

int PartList::getItsCount() const
{
    return itsCount;
}

Part *PartList::getFirst() const
{
    if (pHead)
        return pHead->getItsPart();
    else
        return nullptr;
}
