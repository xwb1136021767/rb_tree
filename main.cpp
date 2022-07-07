#include <iostream>
#include <queue>
#include <random>
//#include <wsman.h>

#include "base/RBTreeNode.h"


class A
{
public:
    A(int a = 0) :ma(a){ std::cout << "A" << std::endl; }
public:
    int ma;
};
class B: virtual public A
{
public:
    B(int b) :A(b), mb(b){ std::cout << "B" << std::endl; }
public:
    int mb;
};
class C :  virtual public A
{
public:
C(int c) :A(c), mc(c){ std::cout << "C" << std::endl; }
public:
int mc;
};
class E
{
public:
    E(int e) :me(e){ std::cout << "E" << std::endl; }
public:
    int me;
};
class D : public B, public C, virtual public E
{
public:
D(int d) :B(d), C(d),E(d),md(d){ std::cout << "D" << std::endl; }
public:
int md;
};



using namespace std;
int main() {

    D d(10);


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
