//
//  lambda.cpp
//  big_project
//
//  Created by András Samu on 13/11/2016.
//  Copyright © 2016 András Samu. All rights reserved.
//

#include "lambda.hpp"
#include <iostream>
#include <sstream>
#include <stack>
using namespace std;

lambda::lambda(string code){
    hasAppl = false;
    currentNode = nullptr;
    root = nullptr;
    fromString(code);
}

lambda::Node* lambda::insertAppl(){
    Node *newAppl = new Node(' ',Appl);
    if (currentNode == nullptr) {
        newAppl->parent = nullptr;
        root = newAppl;
    }else{
        newAppl->parent = currentNode;
        if (currentNode->j_typ == Appl) {
            if (currentNode->left == nullptr) {
                currentNode->left = newAppl;
            }else{
                currentNode->right = newAppl;
            }
        }else{
            currentNode->straight = newAppl;
        }
        
    }
    //cout << "Appl->";
    hasAppl = true;
    return newAppl;
}
lambda::Node* lambda::insertFunc(char label){
    Node *newFunc = new Node(label,Func);
    if (currentNode == nullptr) {
        newFunc->parent = nullptr;
        root = newFunc;
    }else{
        newFunc->parent = currentNode;
        if (currentNode->j_typ == Appl) {
            if (currentNode->left == nullptr) {
                currentNode->left = newFunc;
            }else{
                currentNode->right = newFunc;
            }
        }else{
            currentNode->straight = newFunc;
        }
    }
    //cout<<"Func("<<label<<")->";
    return newFunc;
}
lambda::Node* lambda::insertSubs(char label){
    Node *newSubs = new Node(label,Subs);
    if (currentNode == nullptr) {
        newSubs->parent = nullptr;
        root = newSubs;
    }else{
        newSubs->parent = currentNode;
        if (currentNode->j_typ == Appl) {
            if (currentNode->left == nullptr) {
                currentNode->left = newSubs;
            }else{
                currentNode->right = newSubs;
            }
        }else{
            currentNode->straight = newSubs;
        }
    }
    return newSubs;
    
}
bool lambda::isLeftBrunch(Node *i){
    i = appleChild(i);
    if (i->parent->j_typ == Appl && i->parent->left == i) {
        return true;
    }
    return false;
}
lambda::Node* lambda::appleChild(Node *i){
    while (i->parent->j_typ != Appl) {
        i = i->parent;

    }
    return i;
    
}
void lambda::jumpOneLevelBack(){
    while(currentNode->parent->j_typ != Appl && currentNode != currentNode->parent->right ){
        currentNode = currentNode->parent;
    }
    currentNode = currentNode->parent;

}
void lambda::fromString(std::string strData){
    for (size_t i = 0; i < strData.length(); i++) {
        char a = strData[i];
        if (a == '(') {
            hasAppl = true;
            currentNode = insertAppl();
            continue;
        }
        if(a == ' '){
            //Appl bal gyereke lesz a kovetkezo objektum
            jumpOneLevelBack();
            continue;
        }
        if(a == ')'){
            //Appl bezarult
            jumpOneLevelBack();
            continue;
        }
        if(a == '\\'){
            //Func kezdete
            currentNode = insertFunc(strData[i+1]);
            i++;
            continue;
        }
        if(a == '.'){
            continue;
        }
        currentNode = insertSubs(strData[i]);
    }
}

string lambda::compfy(Node* i){
    if (i != nullptr) {
        if(i->j_typ == Appl){
            return "(";
        }else if(i->j_typ == Func){
            stringstream ss;
            ss << "\\" << i->label <<".";
            return ss.str();
        }else{
            stringstream ss;
            ss << i->label;
            return ss.str();
        }
    }
    return "nullptr ";
}

string lambda::visulaize(Node* i){
    if (i != nullptr) {
        if(i->j_typ == Appl){
            return "Appl -> ";
        }else if(i->j_typ == Func){
            stringstream ss;
            ss << "Func(" << i->label <<") ->";
            return ss.str();
        }else{
            stringstream ss;
            ss << "Subs(" << i->label <<") ->";
            return ss.str();
        }
    }
    return "nullptr ";
}
ostream& lambda::inorderp(Node* i, ostream& o)
{
    stack<Node *> items;
    items.push(i);
    while (!items.empty()) {
        Node *next = items.top();
        items.pop();
        o<<visulaize(next);
        if (next->right != nullptr)
            items.push(next->right);
        if (next->straight != nullptr)
            items.push(next->straight);
        if (next->left != nullptr)
            items.push(next->left);
    }
    return o;

}

ostream& lambda::inorder(ostream& o)
{
    return inorderp(root, o);
}
std::string lambda::to_string_private(Node *i){
    stringstream o;
    stack<Node *> items;
    items.push(i);
    while (!items.empty()) {
        Node *next = items.top();
        items.pop();
        o<<compfy(next);
        if (next->left == nullptr && next->straight == nullptr && next->right == nullptr &&hasAppl&& isLeftBrunch(next)) {
            o << " ";
        }
        if (next->right == nullptr&&next->left == nullptr && next->straight == nullptr && hasAppl&&!isLeftBrunch(next)) {
            o<<") ";
        }
        if (next->right != nullptr){
            items.push(next->right);
        }
        if (next->straight != nullptr){
            items.push(next->straight);
        }
        if (next->left != nullptr){
            items.push(next->left);
        }
    }
    return o.str();

}
std::string lambda::to_string(){
    return to_string_private(root);
}

std::string lambda::to_string_readable(){
    stringstream ss;
    inorder(ss);
    return ss.str();
}
void lambda::reduce(){
    reduce_private(root);
}
void lambda::reduce_private(Node *startNode){
    if(startNode->j_typ == Appl && startNode->left->j_typ == Func){
        evaluate(startNode);
        return;
    }
    if(startNode->left!=nullptr)
    {
        reduce_private(startNode->left);
    }
    if(startNode->straight != nullptr){
        reduce_private(startNode->straight);
    }
    if(startNode->right!=nullptr)
    {
        reduce_private(startNode->right);
    }
}
void lambda::evaluate(Node *StartinAppl){
    Node *funcNode = StartinAppl->left;
    Node *subsNode = find(funcNode, funcNode->label);
    if (subsNode->parent->left == subsNode){
        subsNode->parent->left = funcNode->parent->right;
        funcNode->parent->right->parent = subsNode->parent;
        subsNode->parent = nullptr;
    }else if (subsNode->parent->right == subsNode){
        subsNode->parent->right = funcNode->parent->right;
        funcNode->parent->right->parent = subsNode->parent;
        subsNode->parent = nullptr;
    }else{
        subsNode->parent->straight = funcNode->parent->right;
        funcNode->parent->right->parent = subsNode->parent;
        subsNode->parent = nullptr;
    }
    if (funcNode->parent != root){
        funcNode->straight->parent = funcNode ->parent->parent;
        funcNode ->parent->parent->left = funcNode->straight;
    }
    else{
        root = funcNode->straight;
    }
    //temp->middle = nullptr;
    this->inorder(cout);
    cout<<endl;
    reduce_private(root);
    //cout<< kipakol(gyoker->left);
    
    cout<<endl;

}
lambda::Node* lambda::find(Node *startNode,const char label){
    Node *tmp = nullptr;
    stack<Node *> items;
    items.push(startNode);
    while (!items.empty()) {
        Node *next = items.top();
        items.pop();
        if (label == next->label && next->j_typ == Subs) {
            tmp = next;
        }
        if (next->right != nullptr)
            items.push(next->right);
        if (next->straight != nullptr)
            items.push(next->straight);
        if (next->left != nullptr)
            items.push(next->left);
    }
    return tmp;
}



