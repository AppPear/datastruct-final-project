//
//  main.cpp
//  big_project
//
//  Created by András Samu on 05/11/2016.
//  Copyright © 2016 András Samu. All rights reserved.
//

#include <iostream>
#include "lambda.hpp"
#include <ctime>
using namespace std;

int main() {
  lambda lmd_tree = lambda("((\\b.\\d.(b d) \\c.c) \\a.a)");
  lambda tree_copy = lmd_tree;
  cout<<tree_copy.to_string();
  cout<<endl;
  lmd_tree.inorder(cout);
  cout<<endl;
  lmd_tree.reduce();
  return 0;
}


