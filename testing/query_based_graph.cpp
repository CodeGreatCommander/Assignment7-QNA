#include <iostream>
#include <chrono>
#include <vector>
using namespace std;

// RAKE algorithm basically tokenising the words and removing the stop words
void merge_sort(vector<string>& words,int i,int j){
    if(j<=i)return;
    int mid=(i+j)/2;
    merge_sort(words,i,mid);
    merge_sort(words,mid+1,j);
    vector<string> temp;
    int l=i,r=mid+1;
    while(l<=mid&&r<=j){
        if(words[l]<words[r])
        temp.push_back(words[l++]);
        else
        temp.push_back(words[r++]);
    }
    while(l<=mid)
    temp.push_back(words[l++]);
    while(r<=j)
    temp.push_back(words[r++]);
    l=i;
    for(int i=l;i<=j;i++)
    words[i]=temp[i-l];
}
bool search(vector<string> word,string key){
    int u=word.size()-1,l=0;
    while(l<=u){
        int mid=(l+u)/2;
        if(word[mid]==key)
        return true;
        else if(word[mid]<key)
        l=mid+1;
        else
        u=mid-1;
    }
    return false;
}
vector<string> rake(string sentence){

    vector<string>stop_words=vector<string>{"a", "about", "above", "after", "again", "against", "all", "am", "an", "and",
    "are", "as", "at", "be", "because", "been", "before", "between", "both", "but", "by",
    "can't", "did","do", "does", "doing", "don't", "down", "during", "each", "for", "from",
    "had", "has", "have", "he", "here", "hers", "herself", "him", "himself", "his",
    "i", "if", "in", "into", "is", "it", "its", "itself", "let's", "more", "my",
    "no", "nor", "not", "of", "on", "once", "only", "or", "our", "ours", "ourselves", "out",
    "over", "own", "same", "she", "should", "so", "some", "than", "that", "the", "their",
    "them", "themselves", "then", "there", "these", "they", "this", "through", "to", "too",
    "under", "until", "up", "very","vol", "was", "we", "were", "what", "when", "where", "which",
    "while", "who", "why", "will", "with", "you", "your", "yours", "yourself", "yourselves"};
    //TODO:make a good hash table for stop words
    vector<string> words;
    string word = "",separators=" ,.?!:;\"\'[]{}()";
    for (auto x : sentence) {
        if (separators.find(x) != string::npos) {
            if(word != ""&&!search(stop_words,word))
            words.push_back(word);                
            word = "";
        }
        else {
            word = word + (char)(x<='Z'&&x>='A'?x+32:x); // to lower case
        }
    }
    if(word != ""&&!search(stop_words,word))
    words.push_back(word);
    merge_sort(words,0,words.size()-1);
    vector<string> ans;
    if(words.size()==0)return ans;
    ans.push_back(words[0]);
    for(int i=1;i<words.size();i++){
        if(words[i]!=words[i-1])
        ans.push_back(words[i]);
    }
    return ans;
}
vector<string> merge(vector<string> words,vector<string>word2){
    vector<string>ans;
    int i=0,j=0;
    while(i<words.size()&&j<word2.size()){
        if(words[i]==word2[j]){
            ans.push_back(words[i]);
            i++;
            j++;
        }
        else if(words[i]<word2[j])
        {ans.push_back(words[i++]);}
        else
        {ans.push_back(word2[j++]);}
    }
    while(i<words.size()){
        ans.push_back(words[i++]);
    }
    while(j<word2.size()){
        ans.push_back(word2[j++]);
    }
    return ans;
}


struct Graph_Node{
    vector<string> words;
    int book,para,page;
    Graph_Node(vector<string> words,int book,int para,int page){
        this->words=words;
        this->book=book;
        this->para=para;
        this->page=page;
    }
};
struct Graph{
    double stop_surfing=0.4;//TODO:optimise this value


    vector<Graph_Node*> nodes;
    void add_node(vector<string>&words,int book,int page,int para){
        if(nodes.size()==0||nodes.back()->book!=book||nodes.back()->para!=para||nodes.back()->page!=page)
        nodes.push_back(new Graph_Node(words,book,page,para));
        else{
            nodes.back()->words=merge(nodes.back()->words,words);
        }
    }
    vector<vector<string>> get_words(vector<string>&query){
        vector<vector<string>> ans;
        for(auto node:nodes){
            vector<string> words=node->words,temp=vector<string>();
            for(auto word:query){
                if(search(words,word))
                temp.push_back(word);
            }
            ans.push_back(temp);
        }
        return ans;
    }
    vector<vector<double>> get_adjancency_matrix(vector<vector<string>>&words){
        vector<vector<double>> ans(nodes.size(),vector<double>(nodes.size(),0));
        for(int i=0;i<nodes.size();i++){
            for(int j=0;j<nodes.size();j++){
                if(i==j){
                    ans[i][j]=stop_surfing;
                    continue;
                }
                int x=0,y=0,c=0;
                int i1=0,i2=0;
                while(i1<words[i].size()&&i2<words[j].size()){
                    if(words[i][i1]==words[j][i2]){
                        c++;
                        i1++;
                        i2++;
                    }
                    else if(words[i][i1]<words[j][i2])
                    {x+=2;i1++;}
                    else
                    {y+=2;i2++;}
                }
                x+=2*(words[i].size()-i1);
                y+=2*(words[j].size()-i2);
                ans[i][j]=stop_surfing+(1.0-stop_surfing)*(c+y+2.0)/(words[i].size()+words[j].size()+2.0);
            }
        }
        for(int i=0;i<ans.size();i++){
            double sum=0;
            for(int j=0;j<ans.size();j++){
                sum+=ans[i][j];
            }
            for(int j=0;j<ans.size();j++){
                ans[i][j]/=sum;
            }
        }
        return ans;
    }
    vector<pair<pair<int,pair<int,int>>,double>> get_page_rank(string query){
        vector<string> words=rake(query);
        vector<vector<string>> words_in_nodes=get_words(words);
        vector<vector<double>> adj=get_adjancency_matrix(words_in_nodes);
        // for(int i=0;i<adj.size();i++){
        //     for(int j=0;j<adj.size();j++){
        //         cout<<adj[i][j]<<" ";
        //     }
        //     cout<<endl;
        // }
        // cout<<endl<<endl<<endl;
        vector<double> page_rank(nodes.size(),1.0/nodes.size());
        vector<double> temp(nodes.size(),0);
        for(int i=0;i<30;i++){
            for(int j=0;j<nodes.size();j++){
                temp[j]=0;
                for(int k=0;k<nodes.size();k++){
                    temp[j]+=page_rank[k]*adj[k][j];
                }
            }
            page_rank=temp;
        }
        vector<pair<pair<int,pair<int,int>>,double>> ans;
        for(int i=0;i<page_rank.size();i++){
            ans.push_back({{nodes[i]->book,{nodes[i]->page,nodes[i]->para}},page_rank[i]});
        }
        return ans;
    }
};



// int main(){
//     Graph graph;
//     vector<string> words=rake("what is mahatma gandhi full name");
//     graph.add_node(words,1,1);
//     words=rake("what is ghoda");
//     graph.add_node(words,1,2);
//     vector<pair<pair<int,int>,double>> ans=graph.get_page_rank("what is bolti full");
//     for(auto x:ans){
//         cout<<x.first.first<<" "<<x.first.second<<" "<<x.second<<endl;
//     }
//     return 0;

// }

#define FILENAME "../MKGandhi/mahatma-gandhi-collected-works-volume-"
#include <fstream>
#include <algorithm>
#include <sstream>
int main() {

    // Create a Graph
    Graph graph;
    int count=3;

    for(int file_no=1;file_no<=count;file_no++){
            auto start = chrono::steady_clock::now();
            string filename=FILENAME;
            filename+=to_string(file_no);
            filename+=".txt";
            std::cout<<filename<<endl;
            ifstream file(filename);
            string line;
            int line_count=0;
            int num_length=0;
            int temp=file_no;
            while(temp){
                num_length++;
                temp/=10;
            }
            int last=1;
            std::string tuple;
            std::string sentence;
            while (std::getline(file, tuple, ')') && std::getline(file, sentence)) {
                // Get a line in the sentence
                tuple += ')';

                std::vector<int> metadata;    
                std::istringstream iss(tuple);

                // Temporary variables for parsing
                std::string token;

                // Ignore the first character (the opening parenthesis)
                iss.ignore(1);

                // Parse and convert the elements to integers
                while (std::getline(iss, token, ',')) {
                    // Trim leading and trailing white spaces
                    size_t start = token.find_first_not_of(" ");
                    size_t end = token.find_last_not_of(" ");
                    if (start != std::string::npos && end != std::string::npos) {
                        token = token.substr(start, end - start + 1);
                    }
                    
                    // Check if the element is a number or a string
                    if (token[0] == '\'') {
                        // Remove the single quotes and convert to integer
                        int num = std::stoi(token.substr(1, token.length() - 2));
                        metadata.push_back(num);
                    } else {
                        // Convert the element to integer
                        int num = std::stoi(token);
                        metadata.push_back(num);
                    }
                }
                vector<string> words=rake(sentence);
                graph.add_node(words,metadata[0],metadata[1],metadata[2]);
                line_count++;

            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            // string statements="";
            // while(getline(file,line)){
            //     int i=4+num_length;
            //     string x="";
            //     while(line[i]!=',')x+=line[i++];
            //     int code=stoi(x);
            //     i=0;
            //     while(line[i]!=')')i++;
            //     i+=2;
            //     string sentence=line.substr(i);
            //     // std::cout<<sentence<<endl;
            //     if(last!=code){
            //         vector<string> words=rake(statements);
            //         graph.add_node(words,file_no,line_count+1);
            //         statements="";
            //     }
            //     statements+=sentence;
            //     last=code;
            //     line_count++;
            //     // cout<<file_no<<" "<<code<<" "<<sentence<<endl;
            // }
            file.close();
            auto end = chrono::steady_clock::now();
            std::cout << "Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
        }
        string query="what was mahatma gandhi killed by a child";
        cout<<"Query: "<<query<<endl;
        auto start = chrono::steady_clock::now();
        vector<pair<pair<int,pair<int,int>>,double>> ans=graph.get_page_rank(query);
        sort(ans.begin(),ans.end(),[](pair<pair<int,pair<int,int>>,double> a,pair<pair<int,pair<int,int>>,double> b){
            return a.second>b.second;
        });
        for(int i=0;i<min(10,(int)ans.size());i++){
            cout<<"Page Rank: "<<i<<" Page:"<<ans[i].first.second.first<<" Para:"<<ans[i].first.second.second<<" Book:"<<ans[i].first.first<<" Score:"<<ans[i].second<<endl;
        }
        cout<<endl;
        auto end = chrono::steady_clock::now();
        std::cout << "Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;

    return 0;
}