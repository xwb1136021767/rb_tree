//
// Created by Administrator on 2022/6/23.
//

#ifndef RB_TREE_RBTREENODE_H
#define RB_TREE_RBTREENODE_H
#include <utility>
#include <stack>
#include <Windows.h>


using namespace std;
enum {
    RED,BLACK
};

template<typename K,typename V>
struct RBTreeNode{
    RBTreeNode<K,V>* _left;
    RBTreeNode<K,V>* _right;
    RBTreeNode<K,V>* _parent;

    pair<K,V> _kv;
    int _col;


    RBTreeNode(const pair<K,V> &kv)
       : _left(nullptr),
        _right(nullptr),
        _parent(nullptr),
        _kv(kv),
        _col(RED)
    {

    }

};

template<typename K,typename V>
class RBTree{
private:
    struct RBTreeNode<K,V> *_root;

    void traverse(RBTreeNode<K,V> *_root){
        if(!_root) return;
        traverse(_root->_left);
        cout <<"{ " <<_root->_kv.first << " : " << _root->_kv.second <<" : "<<_root->_col <<"}, ";
        traverse(_root->_right);
    }

    void RotateR(RBTreeNode<K,V> *grandfather);
    void RotateL(RBTreeNode<K,V> *grandfather);
    void RotateLR(RBTreeNode<K,V> *grandfather);
    void RotateRl(RBTreeNode<K,V> *grandfather);
    bool _isRBTree(RBTreeNode<K,V> *root,int count, int blackCount);
    void _print_tree(RBTreeNode<K,V> *root, int depth,int right, int offset);
    int RBtreeDepth(RBTreeNode<K,V> *root);
public:
    RBTree() : _root(nullptr){

    }

    pair<RBTreeNode<K,V>*, bool> Insert(const pair<K, V>& kv);
    void print(){
        traverse(_root);
    }
    void print_level(){
        int level=0;
        const int SPACE=50;
        std::deque<pair<RBTreeNode<K,V>*, int>> stack;
        stack.push_back(make_pair(_root,level));
        int last_level=level;
        cout << endl;
        while(!stack.empty()){
            pair<RBTreeNode<K,V>*, int> tmp = stack.front();
            RBTreeNode<K,V>* node = tmp.first;
            int node_level = tmp.second;
            stack.pop_front();
//            if(level != last_level){ // new line
//                for(int i=0;i<SPACE-2*level;i++){
//                    cout << " ";
//                }
//            }
            if(node_level == last_level)
                cout <<" "<< node->_kv.first<<"  level="<<node_level;
            else{
                last_level = node_level;
                cout << endl;
                cout << " "<<node->_kv.first<<"  level="<<node_level;
            }

            if(node->_left){
                stack.push_back(make_pair(node->_left,node_level+1));
            }
            if(node->_right){
                stack.push_back(make_pair(node->_right,node_level+1));
            }

        }
    }
    bool isRBTree();
    RBTreeNode<K,V>* find(const K& key);

    bool erase(const K& key);

    void print_tree(){
        _print_tree(_root,0,0,0);
    }
};

template<typename K, typename V>
pair<RBTreeNode<K,V>*, bool> RBTree<K, V>::Insert(const pair<K, V> &kv) {
    if(_root == nullptr){
        _root = new RBTreeNode<K,V>(kv);
        _root->_col = BLACK;
        return make_pair(_root,true);
    }

    struct RBTreeNode<K,V> *cur = _root;
    struct RBTreeNode<K,V> *parent = nullptr;

    //1、按二叉搜索树的插入方法，找到待插入位置
    while(cur){
        if(kv.first < cur->_kv.first){
            parent = cur;
            cur = cur->_left;
        }else if(kv.first > cur->_kv.first){
            parent = cur;
            cur = cur->_right;
        }else{
            return make_pair(cur,false);
        }
    }

    //2、将待插入结点插入到树中
    cur = new RBTreeNode<K,V>(kv);
    cur->_col = RED;
    struct RBTreeNode<K,V> *newnode = cur;

    if(kv.first < parent->_kv.first){
        parent->_left = cur;
    }else{
        parent->_right = cur;
    }
    cur->_parent = parent;

    //3、若插入结点的父结点是红色的，则需要对红黑树进行调整
    while(parent && parent->_col == RED){
        struct RBTreeNode<K,V> *grandfather = parent->_parent;
        if(parent == grandfather->_left){
            struct RBTreeNode<K,V> *uncle = grandfather->_right;
            if(uncle && uncle->_col == RED){ //插入结点的叔叔存在，且叔叔的颜色是红色
                grandfather->_col = RED;
                parent->_col = BLACK;
                uncle->_col = BLACK;

                cur = grandfather;
                parent = grandfather->_parent;

            }else{ //插入结点的叔叔存在，且叔叔的颜色是黑色
                if(cur == parent->_left){
                    RotateR(grandfather);
                    parent->_col = BLACK;
                    grandfather->_col = RED;
                }else{
                    RotateLR(grandfather);
                    cur->_col = BLACK;
                    grandfather->_col =RED;
                }
                break;
            }
        }else{ //parent是grandfather的右孩子
            struct RBTreeNode<K,V> *uncle = grandfather->_left;
            if(uncle && uncle->_col == RED){
                grandfather->_col = RED;
                parent->_col = BLACK;
                uncle->_col = BLACK;

                cur = grandfather;
                parent = grandfather->_parent;
            }else{
                    if(cur == parent->_right){
                        RotateL(grandfather);
                        parent->_col = BLACK;
                        grandfather->_col = RED;
                    }else{
                        RotateRl(grandfather);
                        cur->_col = BLACK;
                        grandfather->_col = RED;
                    }
                    break;

            }
        }
    }
    _root->_col = BLACK;
    return make_pair(newnode,true);

}

template<typename K, typename V>
void RBTree<K, V>::RotateR(RBTreeNode<K, V> *grandfather) {

    RBTreeNode<K, V> *gparent = grandfather->_parent;
    RBTreeNode<K, V> *parent = grandfather->_left;
    RBTreeNode<K, V> *parentR = parent->_right;

    //建立parentR与grandfather之间的联系
    grandfather->_left = parentR;
    if(parentR) parentR->_parent = grandfather;

    //建立grandfather与parent之间的联系
    parent->_right = grandfather;
    grandfather->_parent = parent;

    if(gparent == nullptr){
        _root = parent;
        _root->_parent = nullptr;
    }else{
        if(gparent->_left == grandfather){
            gparent->_left = parent;
        }else{
            gparent->_right = parent;
        }
        parent->_parent = gparent;
    }

}

template<typename K, typename V>
void RBTree<K, V>::RotateL(RBTreeNode<K, V> *grandfather) {
    RBTreeNode<K, V> *gparent = grandfather->_parent;
    RBTreeNode<K, V> *parent = grandfather->_right;
    RBTreeNode<K, V> *parentL = parent->_left;

    grandfather->_right = parentL;
    if(parentL) parentL->_parent = grandfather;

    parent->_left = grandfather;
    grandfather->_parent = parent;

    if(gparent == nullptr){
        _root = parent;
        _root->_parent = nullptr;
    }else{
        if(gparent->_left == grandfather){
            gparent->_left = parent;
        }else{
            gparent->_right = parent;
        }

        parent->_parent = gparent;
    }
}

template<typename K, typename V>
void RBTree<K, V>::RotateLR(RBTreeNode<K, V> *grandfather) {
    RotateL(grandfather->_left);
    RotateR(grandfather);
}



template<typename K, typename V>
void RBTree<K, V>::RotateRl(RBTreeNode<K, V> *grandfather) {
    RotateR(grandfather->_right);
    RotateL(grandfather);
}


template<typename K, typename V>
bool RBTree<K, V>::isRBTree() {
    int blackCount=0;
    RBTreeNode<K,V>* tmp=_root;
    while(tmp){
        if(tmp->_col == BLACK){
            blackCount++;
        }
        tmp = tmp->_left;
    }

    return _isRBTree(_root,0,blackCount);
}

template<typename K, typename V>
bool RBTree<K, V>::_isRBTree(RBTreeNode<K,V> *root,int count ,int blackCount) {
    if(!root){
        if(count != blackCount){
            cout << "black node's number is not equal"<<endl;
            return false;
        }
        return true;
    }

    if(root->_col == BLACK)
        count++;

    return _isRBTree(root->_left,count,blackCount) && _isRBTree(root->_right,count,blackCount);

}

template<typename K, typename V>
RBTreeNode<K, V> *RBTree<K, V>::find(const K& key) {
    RBTreeNode<K,V>* tmp=_root;
    while(tmp){
        if(key == tmp->_kv.first){
            return tmp;
        }else if(key < tmp->_kv.first){
            tmp = tmp->_left;
        }else{
            tmp = tmp->_right;
        }
    }
    return nullptr;
}

template<typename K, typename V>
bool RBTree<K, V>::erase(const K &key) {
    // 1. 找到带删除的节点
    RBTreeNode<K,V>* cur = _root;
    RBTreeNode<K,V>* parent = nullptr;
    RBTreeNode<K,V>* del= nullptr;
    RBTreeNode<K,V>* delp= nullptr;
    while(cur){
        if(cur->_kv.first < key){
            parent = cur;
            cur = cur->_right;
        }else if(cur->_kv.first > key){
            parent = cur;
            cur = cur->_left;
        }else{
            if(cur->_left == nullptr){

                if(cur == _root){
                    _root = cur->_right;
                    _root->_col = BLACK;
                    delete cur;
                    return true;
                }

                del = cur;
                delp = parent;
            }else if(cur->_right == nullptr){
                if(cur == _root){
                    _root = cur->_left;
                    _root->_col = BLACK;
                    delete cur;
                    return true;
                }

                del = cur;
                delp = parent;
            }else{
                RBTreeNode<K,V>* minrp = cur;
                RBTreeNode<K,V>* minr = cur->_right;
                while(minr->_left){
                    minrp = minr;
                    minr = minr->_left;
                }

                cur->_kv.first = minr->_kv.first;
                cur->_kv.second = minr->_kv.second;
                del = minr;
                delp = minrp;
            }
            break;
        }
    }

    if(del == nullptr){
        cout << "can't find delete node" << endl;
        return false;
    }

    RBTreeNode<K,V>* real_del = del;
    RBTreeNode<K,V>* real_delp = delp;
    //调整红黑树
    if(del->_col == BLACK){
        if(del->_left){
            del->_left->_col = BLACK;
        }else if(del->_right){
            del->_right->_col = BLACK;
        }else{
            while(del != _root){
                if(del == delp->_left){
                    RBTreeNode<K,V>* brother = delp->_right;
                    if(brother->_col == RED){ // brother为红

                        brother->_col = BLACK;
                        brother->_left->_col = RED;
                        RotateL(delp);
                        break;
                    }else if(brother->_col == BLACK){ //brother为黑
                        if(brother->_left == nullptr && brother->_right){ //brother为黑 且存在右节点
                            brother->_col = delp->_col;
                            delp->_col = BLACK;
                            brother->_right->_col = BLACK;

                            RotateL(delp);
                            break;
                        }else if(brother->_left && brother->_right == nullptr){ //brother为黑 且存在左节点
                            brother->_col = RED;
                            brother->_left->_col = BLACK;
                            RotateR(brother);
                        }else if(brother->_left && brother->_right){ //brother为黑 且存在右节点
                            brother->_col = delp->_col;
                            delp->_col = BLACK;
                            brother->_right->_col = BLACK;

                            RotateL(delp);
                        }else{ //brother为黑 且not存在右节点
                            brother->_col = RED;

                            del = delp;
                            delp = del->_parent;
                        }

                    }
                }else{
                    RBTreeNode<K,V>* brother = delp->_left;
                    if(brother->_col == RED){ // brother为红

                        brother->_col = BLACK;
                        brother->_right->_col = RED;
                        RotateR(delp);
                        break;
                    }else if(brother->_col == BLACK){ //brother为黑
                        if(brother->_left && brother->_right== nullptr){ //brother为黑 且存在left节点
                            brother->_col = delp->_col;
                            delp->_col = BLACK;
                            brother->_right->_col = BLACK;

                            RotateR(delp);
                            break;
                        }else if(brother->_left== nullptr && brother->_right){ //brother为黑 且存在左节点
                            brother->_col = RED;
                            brother->_right->_col = BLACK;
                            RotateL(brother);
                        }else if(brother->_left && brother->_right){ //brother为黑 且存在右节点
                            brother->_col = delp->_col;
                            delp->_col = BLACK;
                            brother->_left->_col = BLACK;

                            RotateR(delp);
                        }else{ //brother为黑 且not存在右节点
                            brother->_col = RED;

                            del = delp;
                            delp = del->_parent;
                        }

                    }
                }
            }
        }
    }

    //实际删除
    if(real_del->_left == nullptr){
        if(real_del == real_delp->_left){
            real_delp->_left = real_del->_right;
            if(real_del->_right)
                real_del->_right->_parent = real_delp;
        }else{
            real_delp->_right = real_del->_right;
            if(real_del->_right)
                real_del->_right->_parent = real_delp;
        }
    }else{
        if(real_del == real_delp->_left){
            real_delp->_left = real_del->_left;
            if(real_del->_right)
                real_del->_right->_parent = real_delp;
        }else{
            real_delp->_right = real_del->_left;
            if(real_del->_right)
                real_del->_right->_parent = real_delp;
        }
    }
    return true;
}


//改变光标位置
void gotoxy(int x, int y)
{
    // 更新光标位置
    COORD pos;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOutput, pos);
}

template<typename K, typename V>
void RBTree<K, V>::_print_tree(RBTreeNode<K, V> *root, int depth, int right, int offset) {
    if(root == nullptr) return;
    int tree_depth = RBtreeDepth(root);
    // 记录当前光标位置，用于在递归中记录当前递归时树的位置
    int x, y;
    // 拆分树，将树的左右子树拆分开来
    RBTreeNode<K,V> *l, *r;
    l = root->_left;
    r = root->_right;

    // 计算父树的偏移量
    int tap1 = offset * pow(2, tree_depth - depth);
    // 计算子树的偏移量
    int tap2 = right * (pow(2, tree_depth - depth));
    // 计算半偏移量
    int tap3 = pow(2, tree_depth - depth - 1);
//    cout << "tap1 = "<< tap1 << endl;
//    cout << "tap2 = "<< tap2 << endl;
//    cout << "tap3 = "<< tap3 << endl;
    // 获取根的坐标
    // x 计算方法为：父偏移量 + 子偏移量 + 半偏移量 - 1
    // y 计算方法为：目前打印的层数 * 2
    x = tap1 + tap2 + tap3 - 1;
    y = depth * 2;

    // 打印根的位置
    gotoxy(x * 2, y);
    printf("%d", root->_kv.first);

    // 在打印子树时，当前层数+1
    depth++;
    // 计算子树的父偏移量
    offset = offset * 2 + (right == 1 ? 2 : 0);

    if (l == NULL && r == NULL) return;
    else if (r == NULL) {
        // 打印左子树的位置
        gotoxy(x * 2 - tap3, y + 1);
        printf("┏");
        for (int i = 0; i < tap3-1; i++) printf("━");
        printf("┛");
        _print_tree(l, depth, 0, offset);
    }
    else if (l == NULL) {
        // 打印右子树的位置
        gotoxy(x * 2, y + 1);
        printf("┗");
        for (int i = 0; i < tap3-1; i++) printf("━");
        printf("┓");
        _print_tree(r, depth, 1, offset);
    }
    else {
        // 打印左右子树的位置
        gotoxy(x * 2 - tap3, y + 1);
        printf("┏");
        for (int i = 0; i < tap3 - 1; i++) printf("━");
        printf("┻");
        for (int i = 0; i < tap3 - 1; i++) printf("━");
        printf("┓");
        _print_tree(l, depth, 0, offset);
        _print_tree(r, depth, 1, offset);
    }


}



template<typename K, typename V>
int RBTree<K, V>::RBtreeDepth(RBTreeNode<K, V> *root) {
    int level=0;
    std::deque<pair<RBTreeNode<K,V>*, int>> stack;
    stack.push_back(make_pair(_root,level));
    int depth=0;
    while(!stack.empty()){
        pair<RBTreeNode<K,V>*, int> tmp = stack.front();
        RBTreeNode<K,V>* node = tmp.first;
        int node_level = tmp.second;
        depth = max(depth,node_level);
        stack.pop_front();
        if(node->_left){
            stack.push_back(make_pair(node->_left,node_level+1));
        }
        if(node->_right){
            stack.push_back(make_pair(node->_right,node_level+1));
        }
    }
    return depth;
}


#endif //RB_TREE_RBTREENODE_H
