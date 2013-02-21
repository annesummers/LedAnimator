#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include <QObject>
#include <vector>
#include <map>
typedef std::vector<bool> HuffCode;
typedef std::map<char, HuffCode> HuffCodeMap;

const int UniqueSymbols = 1 << 8;

class INode
{
public:
    const int f;

    virtual ~INode() {}

protected:
    INode(int f) : f(f) {}
};

class InternalNode : public INode
{
public:
    INode *const left;
    INode *const right;

    InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}
    ~InternalNode()
    {
        delete left;
        delete right;
    }
};

class LeafNode : public INode
{
public:
    const char c;

    LeafNode(int f, char c) : INode(f), c(c) {}
};

struct NodeCmp
{
    bool operator()(const INode* lhs, const INode* rhs) const { return lhs->f > rhs->f; }
};

class HuffmanCode : public QObject
{
    Q_OBJECT
public:
    explicit HuffmanCode(QObject *parent = 0);

    void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes);
    INode*  BuildTree(const int* frequencies);
    
signals:
    
public slots:
    
};

#endif // HUFFMANCODE_H
