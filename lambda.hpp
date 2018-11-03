//
//  lambda.hpp
//  big_project
//
//  Created by András Samu on 13/11/2016.
//  Copyright © 2016 András Samu. All rights reserved.
//

#ifndef lambda_hpp
#define lambda_hpp

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <ctime>

class lambda
{
    enum junction_typ{
        Func, Appl, Subs
    };
    enum side{
        left,right
    };
    class Node
    {
    public:
        junction_typ j_typ;
        Node* parent;
        Node *left,*right,*straight;
        char label;
        Node(char k, junction_typ typ) : j_typ(typ), parent(nullptr ), left(nullptr ), right(nullptr ),straight(nullptr), label(k){
}
        Node(char k, Node* p, junction_typ typ) : j_typ(typ), parent(p), left(nullptr ), right(nullptr ),straight(nullptr), label(k) {
}
    };
    Node* root;
    Node* currentNode;
    
    Node* insertAppl();
    Node* insertFunc(char label);
    Node* insertSubs(char label);
    void fromString(std::string strData);
    void jumpOneLevelBack();
    std::string visulaize(Node* i);
    std::string compfy(Node* i);
    std::ostream& inorderp(Node* i, std::ostream& o);
    void reduce_private(Node *startNode);
    void evaluate(Node *StartinAppl);
    Node* find(Node *startNode,const char label);
    std::string to_string_private(Node *i);
    bool isLeftBrunch(Node *i);
    Node* appleChild(Node *i);
    bool hasAppl;

public:
    std::ostream& inorder(std::ostream& o);
    lambda(std::string code);
    std::string to_string();
    std::string to_string_readable();

    void reduce();
};



#endif /* lambda_hpp */
