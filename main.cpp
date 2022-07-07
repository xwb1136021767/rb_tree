#include <iostream>
#include <queue>
#include <random>
//#include <wsman.h>

#include "base/RBTreeNode.h"


class myClass {
public:
    myClass& operator->() {
        std::cout << "myClass ->() is called!" << std::endl;
        return *this;
    }
    void action() {
        std::cout << "myClass action() is called!" << std::endl;
        return;
    }
};

class Animal {
public:
    virtual void cry()
    {
        std::cout << "Animal cry" << std::endl;
    }
};

class Dog : public Animal{
public:
    void look()
    {
        std::cout << "Dog look " << std::endl;
    }
};

class Cat : public Animal{
public:
    void look()
    {
        std::cout << "Cat look " << std::endl;
    }
};

using namespace std;
int main() {
    Animal* base = new Dog();
    base->cry();

    Dog* dog = dynamic_cast<Dog*>(base);
    if(dog)
        cout << "success" << endl;
    else
        cout << "failure" << endl;


    try{
        Cat& cat = dynamic_cast<Cat&>(*base);

    }catch (std::bad_cast){
        cout << "bad_cast"<<endl;
    }



//    RBTree<int,int> root;
//    pair<int,int> node(1,2);
//
//    mt19937 mt_rand(random_device{}());
//    uniform_int_distribution<int> scope(0,100);
//    for(int i=0;i<20;i++){
//        node.first = scope(mt_rand);
//        node.second = scope(mt_rand);
//        root.Insert(node);
//    }
//
//    root.print_tree();
//        root.Insert(node);
//    root.print();
//    root.print_level();
//    cout << endl;
//    cout << root.isRBTree()<<endl;
//
//    for(int i=0;i<100;i++){
//        node.first = scope(mt_rand);
//        node.second = scope(mt_rand);
//        cout << "erase : "<<node.first << endl;
//        if(root.erase(node.first)){
//            root.print_level();
//        }
//    }
    return 0;
}
