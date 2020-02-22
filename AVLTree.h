//
// Created by Yongjia Xu on 2/10/20.
//

#ifndef INC_20S_3353_PA01_AVLTREE_H
#define INC_20S_3353_PA01_AVLTREE_H
#include <iostream>
template <typename T>
class AVLTree
{

private:

    // AVLNode
    class AVLNode{
    public:
        T element;
        AVLNode *left;
        AVLNode *right;
        int height;
        AVLNode(const T & theElement, AVLNode *lt, AVLNode *rt, int h = 0) :
                element(theElement), left(lt), right(rt), height(h){}
    };

    //data member
    int size;
    AVLNode *root;
    T* duplicate;

    // helper funcs
    int height(AVLNode *t) const{ return t == nullptr ? -1 : t->height;}
    int max(int lhs, int rhs) const{ return lhs > rhs ? lhs : rhs;}

    // recursive funcs
    void clear(AVLNode*& curr);
    void print(AVLNode *& curr);
    void insert(const T & x, AVLNode *& t);
    const T& find(const T&, const AVLNode*) const;
    void outputHelper(std::ostream& o, AVLNode* curr) const; // for printing

    // rotation funcs
    void rotateWithLeftChild(AVLNode *& k2);
    void rotateWithRightChild(AVLNode *& k1);
    void doubleWithLeftChild(AVLNode *& k2);
    void doubleWithRightChild(AVLNode *& k1);

    // for copying
    AVLNode * clone (AVLNode *t) const{
        if(t == nullptr) return nullptr;
        else return new AVLNode(t->element, clone(t->left), clone(t->right), t->height);
    }

public:

    AVLTree():  size(0), root(nullptr), duplicate(nullptr){} // default constructor
    AVLTree(const AVLTree & rhs) : root(nullptr), size(0), duplicate(nullptr){*this = rhs;} // copy constructor
    ~AVLTree(){clear();} // destructor
    const AVLTree& operator=(const AVLTree& rhs){  // overloaded assignment operator
        if(this != &rhs){
            clear();
            size = rhs.size;
            root = clone(rhs.root);
            duplicate = nullptr;
        }
        return *this;
    }

    // node finder
    const T& find(const T& x) const{
        if(isEmpty()) throw std::underflow_error("The avltree is empty");
        return find(x, root);
    }

    // getters
    int getSize() const{return size;}
    T* getDuplicate(){return duplicate;}

    // checkers
    bool contains(T& x) const{return contains(x, root);}
    bool isEmpty() const{return root == nullptr;}

    // interactive funcs
    void clear(){clear(root);}
    void print(){print(root);} // cout printing
    void insert(const T & x){duplicate = nullptr;insert(x,root);}
    void outputHelper(std::ostream& o) const {outputHelper(o,root);}
};

template <typename T>
void AVLTree<T>::clear(AVLNode*& curr){
    if(curr!= nullptr){
        clear(curr->left);
        clear(curr->right);
        delete curr;
    }
    curr = nullptr;
}

template <typename T>
void AVLTree<T>::print(AVLNode *& curr){
    if(curr != nullptr){
        print(curr->left);
        std::cout << curr->element << " ";
        print(curr->right);
    }
}

template <typename T>
// duplicate value made accessble to other class to be handled
void AVLTree<T>::insert(const T & x, AVLNode *& t){
    if(t == nullptr){
        t = new AVLNode(x, nullptr, nullptr);
        ++size;
    }
    else if(x < t->element){
        insert(x, t->left);
        if(height(t->left) - height(t->right) == 2){
            if(x < t->left->element) rotateWithLeftChild(t);
            else doubleWithLeftChild(t);
        }
    }
    else if(x > t->element){
        insert(x, t->right);
        if(height(t->right) - height(t->left) == 2){
            if( t->right->element < x) rotateWithRightChild(t);
            else doubleWithRightChild(t);
        }
    }
    else{ // when inserts a duplicate value
        duplicate = &(t->element);
    }
    t->height = max(height(t->left), height(t->right))+1;
}

template <typename T>
const T& AVLTree<T>::find(const T& x, const AVLNode* t) const{
    if(x == t->element) return t->element;
    else if(x > t->element){
        if(t->right == nullptr) return t->element;
        else return find(x, t->right);
    }
    else if(x < t->element){
        if(t->left == nullptr) return t->element;
        else return find(x, t->left);
    }
}

template <typename T>
void AVLTree<T>::outputHelper(std::ostream& o, AVLNode* curr) const{
    if(curr != nullptr){
        o << curr->element << std::endl;
        outputHelper(o,curr->left);
        outputHelper(o,curr->right);
    }
}

template <typename T>
void AVLTree<T>::rotateWithLeftChild(AVLNode *& k2){
    AVLNode * k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template <typename T>
void AVLTree<T>::rotateWithRightChild(AVLNode *& k1){
    AVLNode * k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(k1->height, height(k2->right)) + 1;
    k1 = k2;
}

template <typename T>
void AVLTree<T>::doubleWithLeftChild(AVLNode *& k3){
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <typename T>
void AVLTree<T>::doubleWithRightChild(AVLNode *& k4){
    rotateWithLeftChild(k4->right);
    rotateWithRightChild(k4);
}


#endif //INC_20S_3353_PA01_AVLTREE_H
