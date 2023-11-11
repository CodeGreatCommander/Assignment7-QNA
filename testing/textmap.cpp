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


//Graph
int calcualte_similarity(vector<string>&w1,vector<string>&w2){
    int i=0,j=0,ans=0;
    while(i<w1.size()&&j<w2.size()){
        if(w1[i]==w2[j]){
            ans++;
            i++;
            j++;
        }
        else if(w1[i]<w2[j])
        i++;
        else
        j++;
    }
    return ans;
}
struct Graph_Node{
    vector<string> words;
    int book,para;
    Graph_Node(vector<string> words,int book,int para){
        this->words=words;
        this->book=book;
        this->para=para;
    }
};
struct Graph{
    vector<Graph_Node*> nodes;
    vector<vector<double>> adj;
    vector<double> page_rank;
    void add_node(vector<string> words,int book,int para){
        Graph_Node* temp=new Graph_Node(words,book,para);
        adj.push_back(vector<double>());
        for(int i=0;i<nodes.size();i++){
            int similarity=calcualte_similarity(words,nodes[i]->words);
            adj[i].push_back(0.4+0.6*similarity);
            adj[adj.size()-1].push_back(0.4+0.6*similarity);

        }
        adj[adj.size()-1].push_back(0.4);
        nodes.push_back(temp);
    }
    void normalise(){
        for(int i=0;i<nodes.size();i++){
            double sum=0;
            for(int j=0;j<nodes.size();j++){
                sum+=adj[i][j];
            }
            for(int j=0;j<nodes.size();j++){
                adj[i][j]/=sum;
            }
        }
    }
    void calculate_page_rank(){
        normalise();
        page_rank=vector<double>(nodes.size(),1.0/nodes.size());
        double total=0;
        for(int _=0;_<49;_++){
            for(int i=0;i<nodes.size();i++){
                double sum=0;
                for(int j=0;j<nodes.size();j++){
                    sum+=adj[j][i]*page_rank[j];
                }
                if(sum>page_rank[i]){
                    page_rank[i]=sum;
                }
            }
        }
    }
    vector<pair<pair<int,int>,double>> get_page_rank(){
        vector<pair<pair<int,int>,double>> ans;
        for(int i=0;i<nodes.size();i++){
            ans.push_back({{nodes[i]->book,nodes[i]->para},page_rank[i]});
        }
        return ans;
    }
    void print_all(){
        for(int i=0;i<adj.size();i++){
            for(int j=0;j<adj.size();j++){
                std::cout<<adj[i][j]<<" ";
            }
            std::cout<<endl;
        }
        std::cout<<endl;
    }
    ~Graph(){
        for(int i=0;i<nodes.size();i++){
            delete nodes[i];
        }
    }
};

// #define FILENAME "../MKGandhi/mahatma-gandhi-collected-works-volume-"
// #include <fstream>
// int main() {

//     // Create a Graph
//     Graph graph;
//     int count=1;

//     for(int file_no=1;file_no<=count;file_no++){
//             auto start = chrono::steady_clock::now();
//             string filename=FILENAME;
//             filename+=to_string(file_no);
//             filename+=".txt";
//             std::cout<<filename<<endl;
//             ifstream file(filename);
//             string line;
//             int line_count=0;
//             int num_length=0;
//             int temp=file_no;
//             while(temp){
//                 num_length++;
//                 temp/=10;
//             }
//             int last=1;
//             string statements="";
//             while(getline(file,line)&&line_count<10000){
//                 int i=4+num_length;
//                 string x="";
//                 while(line[i]!=',')x+=line[i++];
//                 int code=stoi(x);
//                 i=0;
//                 while(line[i]!=')')i++;
//                 i+=2;
//                 string sentence=line.substr(i);
//                 // std::cout<<sentence<<endl;
//                 if(last!=code){
//                     vector<string> words=rake(statements);
//                     graph.add_node(words,file_no,line_count+1);
//                     statements="";
//                 }
//                 statements+=sentence;
//                 last=code;
//                 line_count++;
//                 // cout<<file_no<<" "<<code<<" "<<sentence<<endl;
//             }
//             file.close();
//             auto end = chrono::steady_clock::now();
//             std::cout << "Elapsed time in milliseconds: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms" << endl;
//         }
//         graph.calculate_page_rank();
//         vector<double> page_rank=graph.get_page_rank();
//         cout<<"Page Rank of all the nodes"<<endl<<page_rank.size();

//     return 0;
// }
