#include <iostream>
#include <vector>
#include <map>

#define TWO_LINK 2
#define THREE_LINK 3
#define FOUR_LINK 4

using namespace std;

class Node 
{
protected:
    int key[3];  //为4-点准备 空间 存储值
    string value[3];  //为4-点准备 空间 存储值

    vector<Node*> node_list;  //最多四条  分别对应 4- 节点的 四条 子

public:
    bool put(Node** node, int key, string value);
    string search_node(Node* node, int key);
    Node();
    Node(int key, string value);
    ~Node();

private:
    int flag_node;
    string search_two(Node* node, int key);
    string search_three(Node *node, int key);
    Node* deal_links(Node** child, Node **now, int key, string value, int depth);
    void add_second_node(Node** now, int key, string value);
    void add_third_node(Node** now, int key, string value);
    Node* deal_four_links(Node** child, Node** now);
    void deal_top_tree(Node** now);
    Node* put_node(Node** node, int key, string value, int depth);
    Node* put_two(Node** node, int key, string value, int depth);
    Node* put_three(Node** node, int key, string value, int depth);
};