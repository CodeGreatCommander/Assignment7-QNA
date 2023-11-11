#include "extra_data_structures.cpp"
#define FILENAME "../MKGandhi/mahatma-gandhi-collected-works-volume-"
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono>
#include "textmap.cpp"
using namespace std;

struct Part_1{
    int count =2;//Edit this for changing number of files
    Graph graph;
    Tries<pair<int,int>,int> trie;
    void extract_csv(){
        auto start = std::chrono::high_resolution_clock::now();
        ifstream file("../unigram_freq.csv");
        string line;
        getline(file,line);
        while(getline(file,line)){
            int i=0;
            string x="";
            while(line[i]!=',')x+=line[i++];
            string word=x;
            i++;
            long long int number=0;
            while(i<line.size()){
                number=number*10+(line[i]-'0');
                i++;
            }
            trie.insert(word,number,0);
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Time taken by the operation: " << duration.count() << " microseconds" << std::endl;
    }
    void extract_file(string sentence,int book,int code){
        string ans="";
      	for (int i=0,l=sentence.size();i<l;i++){
            string x=" .,-:!\"\'()?—[]“”‘’˙;@";
            if(x.find(sentence[i])!=string::npos){
                if(ans.size()!=0)
				trie.increase_by_1(ans,{book,code},0);
				ans="";	
            }
	    else ans+=sentence[i]>='A'&&sentence[i]<='Z'?(char)(sentence[i]-'A'+'a'):sentence[i];//check complexity and handle upper case number issues
        }
        if(ans.size()!=0)
        trie.increase_by_1(ans,{book,code},0);
        return;
    }
    void insert_corpus(){
        auto start = std::chrono::high_resolution_clock::now();
        for(int file_no=1;file_no<=count;file_no++){
            string filename=FILENAME;
            filename+=to_string(file_no);
            filename+=".txt";
            cout<<filename<<endl;
            ifstream file(filename);
            string line;
            int line_count=0;
            int num_length=0;
            int temp=file_no;
            string present="";
            int id=1;
            while(temp){
                num_length++;
                temp/=10;
            }
            while(getline(file,line)){
                int i=4+num_length;
                string x="";
                while(line[i]!=',')x+=line[i++];
                int code=stoi(x);
                i=0;
                while(line[i]!=')')i++;
                i+=2;
                string sentence=line.substr(i);
                extract_file(sentence,file_no,code);
                if(code!=id){
                    vector<string> words=rake(present);
                    graph.add_node(words,file_no,id);
                    present="";
                }
                present+=sentence;
                id=code;
                line_count++;
                // cout<<file_no<<" "<<code<<" "<<sentence<<endl;
            }
            vector<string> words=rake(present);
            graph.add_node(words,file_no,id);
            file.close();
        }
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        std::cout << "Time taken by the operation: " << duration.count() << " microseconds" << std::endl;
    }
    Part_1(){
        insert_corpus();
        extract_csv();
        graph.calculate_page_rank();
    }
    void update_avl(AVLMap<pair<int,int>,double>& scores,vector<pair<pair<int,int>,int>>& v,double total){
        for(auto x:v){
            scores.increase_by_x(x.first,x.second*total);
        }
    }
    double sqrt(double x){
        double l=0,r=x;
        while(r-l>1e-12){
            double m=(l+r)/2;
            if(m*m>x)r=m;
            else l=m;
        }
        return l;
    }
    int get_top_k(vector<pair<pair<int,int>,double>>&v){
        double m=0,ms=0;
        for(auto x:v){
            m+=x.second;
            ms+=x.second*x.second;
        }
        m/=v.size();
        ms/=v.size();
        cout<<"mean:"<<m<<endl;
        ms-=m*m;
        ms=sqrt(ms);
        cout<<"gadient:"<<ms<<endl;
        int k=1;
        while(k<v.size()&&(-v[k].second+v[k-1].second)>1.5*ms)k++;
        return min(k,20);

    }
    void compute(string query){
        string ans="";
        AVLMap<pair<int,int>,double> scores;
      	for (int i=0,l=query.size();i<l;i++){
            string x=" .,-:!\"\'()?—[]“”‘’˙;@";
            if(x.find(query[i])!=string::npos){
                if(ans.size()!=0){
                    Tries<pair<int,int>,int>* temp=trie.get(ans,0);
                    if(temp){
                        vector<pair<pair<int,int>,int>> v;
                        temp->data.get_all(v);
                        update_avl(scores,v,(temp->total+1.0)/(temp->c_val+1.0));
                    }
                }
				ans="";	
            }
	        else ans+=query[i]>='A'&&query[i]<='Z'?(char)(query[i]-'A'+'a'):query[i];//check complexity and handle upper case number issues
        }
        if(ans.size()!=0){
            Tries<pair<int,int>,int>* temp=trie.get(ans,0);
            if(temp){
                vector<pair<pair<int,int>,int>> v;
                temp->data.get_all(v);
                update_avl(scores,v,(temp->total+1.0)/(temp->c_val+1.0));
            }
        }
        vector<pair<pair<int,int>,double>> v;
        scores.get_all(v);
        vector<pair<pair<int,int>,double>> page_rank=graph.get_page_rank();
        cout<<page_rank.size()<<endl<<v.size()<<endl;
        for(int i=0,j=0;i<v.size();j++){
            if(j<page_rank.size()&&page_rank[j].first==v[i].first){
                v[i].second+=page_rank[j].second;
                i++;
            }
            else if(j>=page_rank.size()){
                v[i].second=0;
            }
        }
        sort(v.begin(),v.end(),[](pair<pair<int,int>,double> a,pair<pair<int,int>,double> b){
            return a.second>b.second;
        });
        for(int i=0;i<min((int)v.size(),5);i++){
            cout<<v[i].first.first<<" "<<v[i].first.second<<" "<<v[i].second<<endl;
        }
        int l=get_top_k(v);
        cout<<l<<endl;
        for(int i=0;i<l;i++){
            cout<<v[i].first.first<<" "<<v[i].first.second<<" "<<v[i].second<<endl;
        }
    }
};

int main(){
    Part_1 p;
    cout<<"next question?"<<"what should i learn from life"<<endl;
    p.compute("what should i learn from life");
    cout<<"next question?"<<"Mahatma Gandhi biggest mistake"<<endl;
    p.compute("Mahatma Gandhi biggest mistake");
    return 0;
}