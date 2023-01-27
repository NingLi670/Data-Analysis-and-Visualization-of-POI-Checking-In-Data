#ifndef KDTREE_H
#define KDTREE_H

#include <QVector>
#include <QDebug>

struct Item{
    double key[2];
    int value;
};

class Node{
public:
    Item *item;
    Node *left,*right;
    Node(Item *_item):item(_item),left(nullptr),right(nullptr){}
};

class kdtree{
    Node *root;
    QVector<Item*> *vec;

public:
    QVector<int> pois;
    kdtree(QVector<Item*> *_vec):root(nullptr),vec(_vec){
    }
    void build();
    void insert(Node *&root,Item *item,int dim);
    void get(Node *root,double min[],double max[],int dim);
    void getpois(double min[],double max[]);
    void print(Node *root);
};

#endif // KDTREE_H
