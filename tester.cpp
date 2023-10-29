#include<bits/stdc++.h>
#include "Node.h"
#include "qna_tool.h"

using namespace std;

#define FILENAME "mahatma-gandhi-collected-works-volume-1.txt"

int main(){
    QNA_tool qna_tool;
    qna_tool.query("What is the purpose of life?", "answer.txt");
    return 0;
}