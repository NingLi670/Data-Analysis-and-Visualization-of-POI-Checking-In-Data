#include "kdtree.h"


void kdtree::insert(Node*& root, Item *item, int dim) {
    if (root == NULL) {
        root = new Node(item);
        return;
    }
    if (item->key[dim] < root->item->key[dim])
        insert(root->left, item, (dim + 1) % 2);
    else
        insert(root->right, item, (dim + 1) % 2);
}
void kdtree::print(Node *root){
    if(!root) return;
    print(root->left);
    print(root->right);
}
void kdtree::build(){
    for(int i=0;i<vec->size();i++){
        insert(root,vec->at(i),0);
    }
}

void kdtree::get(Node *root,double min[],double max[],int dim){
    if(!root) return;
    int d=(dim+1)%2;
        if(root->item->key[dim]<min[dim]){
            get(root->right,min,max,d);
            return;
        }
        else if(root->item->key[dim]>max[dim]){
            get(root->left,min,max,d);
            return;
        }
        else{
            if(root->item->key[d]>=min[d]&&root->item->key[d]<=max[d]){
                pois.push_back(root->item->value);
            }
            get(root->left,min,max,d);
            get(root->right,min,max,d);
        }
}
void kdtree::getpois(double min[],double max[]){
    pois.clear();
    get(root,min,max,0);
}
