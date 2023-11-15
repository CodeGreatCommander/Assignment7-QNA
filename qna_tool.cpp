#include <assert.h>
#include <sstream>
#include "qna_tool.h"

using namespace std;

//Trie and AVL implementation
template <class T,class X>
class AVLMap{
private:
	struct Node{
		T d;
		X v;
		int height;
		Node* left,*right;
		Node(T d,X v):d(d),v(v),height(0),left(nullptr),right(nullptr){}
		~Node(){if(left)delete left;if(right)delete right;}
		Node* balance(){
			if(abs(get_height(left)-get_height(right))<2)return this;
			else if(get_height(left)>get_height(right)&&get_height(left->right)>get_height(left->left)){
				left=left->rr();
				return this->ll();
			}
			else if(get_height(left)>get_height(right))return this->ll();
			else if(get_height(right->right)>get_height(right->left))return this->rr();
			else {
				right=right->ll();
				return this->rr();
			}
		}
		Node* rr(){//Right Rotation 
			if(!right)return this;
			Node* temp=right;
			this->right=temp->left;
			temp->left=this;
			update_height(this);
			update_height(temp);
			return temp;
		}
		Node* ll(){//Left Rotation
			if(!left)return this;
			Node* temp=left;
			this->left=temp->right;
			temp->right=this;
			update_height(this);
			update_height(temp);
			return temp;
		}
		void update_height(){
			height=max(get_height(left),get_height(right))+1;
		}
		private:
		int get_height(Node* r){
			if(!r)return -1;
			else return r->height;
		}
		void update_height(Node* r){
			if(!r)return;
			r->height=max(get_height(r->left),get_height(r->right))+1;
		}
	};
	Node* root;
	Node* insert(T d,X v,Node* r){
		if(!r)return new Node(d,v);
		if(r->d>d){
			r->left=insert(d,v,r->left);
		}
		else{
			r->right=insert(d,v,r->right);
		}
		r->update_height();
		return r->balance();
	}
	Node* find(T d,Node* r){
		if(!r)return nullptr;
		else if(r->d==d)return r;
		else if(r->d<d)return find(d,r->right);
		else return find(d,r->left);
	}
	void get_all(Node* r,vector<pair<T,X>>& ans){
		if(!r)return;
		get_all(r->left,ans);
		ans.push_back({r->d,r->v});
		get_all(r->right,ans);
	}
public:
	AVLMap(){
		root=nullptr;
	}
	~AVLMap(){
		delete root;
	}
	void insert(T d,X v){
		Node* x=find(d,root);
		if(!x)root=insert(d,v,root);
		else x->v=v;
	}
	bool exist(T d){
		return find(d,root)!=nullptr;
	}
	Node* find(T d){
		return find(d,root);
	}
	X get(T d){

		Node* x=find(d,root);
		if(!x)return NULL;
		else return x->v;
	}
	void increase_by_x(T d,X v){
		Node* x=find(d,root);
		if(!x)insert(d,v);
		else x->v+=v;
	}	
	void get_all(vector<pair<T,X>>& ans){
		get_all(root,ans);
	}
};


//Trie Implementation
template <class t,class d>
class Tries{
public:
	long long int total;
	AVLMap<char,Tries*> child;
	AVLMap<t,d> data;
	long long int c_val;
	Tries():total(0),c_val(0){}
	void insert(string x,long long int c_val,int i){//initially pass parameter 0
		if(i==x.length()){this->c_val=c_val;return;}
		Tries* f=child.get(x[i]);
		if(!f){
			child.insert(x[i],new Tries());
			f=child.get(x[i]);
		}
		f->insert(x,c_val,i+1);
	}
	void insert(string x,t a,d b,int i){//initially pass paramter 0
		if(i==x.length()){data.insert(a,b);return;}
		Tries* f=child.get(x[i]);
		if(!f){
			child.insert(x[i],new Tries());
			f=child.get(x[i]);
		}
		f->insert(x,a,b,i+1);
	}
	Tries* get(string x,int i){
		if(i==x.length()){return this;}
		Tries* f=child.get(x[i]);
		if(!f){
			return nullptr;
		} 
		return f->get(x,i+1);
	}
	void increase_by_1(string x,t a,int i){
		if(i>=x.length()){
			total+=1;
			if(!data.exist(a))
			data.insert(a,1);
			else{//check this part
				data.find(a)->v=data.find(a)->v+1;
			}
			return;
		}
		Tries* f=child.get(x[i]);
		if(!f){
			child.insert(x[i],new Tries());
			f=child.get(x[i]);
		}
		f->increase_by_1(x,a,i+1);
	}
};

template <class T>
class Heap{
	vector<T> heap;
	size_t size;
	size_t par(size_t i){
		return (i-1)/2;
	}
	void swap(T& a,T&b){
		T c=a;
		a=b;
		b=c;
	}
	size_t left(size_t i){
		return 2*i+1;
	}
	size_t right(size_t i){
		return 2*i+2;
	}
public:
	Heap():size(0){}
	void insert(T x){
		if(size==0){heap.push_back(x);size++;return;}
		heap.push_back(x);
		size++;
		size_t i=size-1;
		while(i>0){
			if(heap[par(i)]>heap[i]){
				swap(heap[par(i)],heap[i]);
				i=par(i);
			}
			else break;
		}
	}
	void pop(){
		if(size==0)return;
		heap[0]=heap.back();
		heap.pop_back();
		size--;
		int i=0;
		while(left(i)<size){
			if(right(i)>=size){
				if(heap[left(i)]<heap[i]){
					swap(heap[left(i)],heap[(i)]);
				}
				break;
			}
			else{
				if(heap[left(i)]<heap[right(i)]&&heap[i]>heap[left(i)]){swap(heap[left(i)],heap[i]);i=left(i);}
				else if(heap[left(i)]>=heap[right(i)]&&heap[i]>heap[right(i)]){swap(heap[right(i)],heap[i]);i=right(i);}
				else break;
			}
		}
	}
	T get_top(){
		if(size==0)return T();
		return heap[0];
	}
    size_t get_size(){
        return size;
    }
    void print(){
        for(int i=0;i<size;i++){
            cout<<heap[i]<<" ";
        }
        cout<<endl;
    }
};


//Rake Implementation
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
vector<pair<string,int>> rake(string sentence){

    vector<string>stop_words=vector<string>{"a", "about", "above", "after", "again","all", "am", "an", "and",
    "are", "as", "at", "be", "because", "been", "before", "both", "but", "by",
    "can't", "did","do", "does", "don't", "down", "during", "each", "for", "from","gandhi","gandhis",
    "had", "has", "have", "he", "here", "hers", "herself", "him", "himself", "his","how",
    "i", "if", "in", "into", "is", "it", "its", "itself", "let's","mahatma","mahatmas", "my",
    "no", "nor", "not", "of", "on", "or", "our", "ours", "ourselves", "out",
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
    vector<pair<string,int>> ans;
    if(words.size()==0)return ans;
    ans.push_back({words[0],1});
    for(int i=1;i<words.size();i++){
        if(words[i]!=words[i-1])
        ans.push_back({words[i],1});
        else ans.back().second++;
    }
    return ans;
}
struct Graph_Node{
    int book_code,page,paragraph,total_words;
    vector<pair<string,int>> words;
};
struct Graph{
    vector<Graph_Node*> nodes;
    Graph_Node* search(int b_c,int pag,int par){
        for(auto x:nodes){
            if(x->book_code==b_c&&x->page==pag&&x->paragraph==par)
            return x;
        }
        return nullptr;
    }
    void add_node(int book_code,int page,int paragraph,pair<string,int> word,int total_words){
        Graph_Node* check=search(book_code,page,paragraph);
        if(check){
            check->words.push_back(word);
            return;
        }
        Graph_Node* temp=new Graph_Node();
        temp->book_code=book_code;
        temp->page=page;
        temp->paragraph=paragraph;
        temp->words.push_back(word);
        temp->total_words=total_words;
        nodes.push_back(temp);
    }
    int compare(const vector<pair<string,int>>& a,const vector<pair<string,int>>&b) const{
        int i=0,j=0;
        int score=0;
        while(i<a.size()&&j<b.size()){
            if(a[i].first==b[j].first){
                i++;
                j++;
                score+=a[i].second;
            }
            else if(a[i]<b[j]){score+=3*a[i].second;i++;}//TODO: improve this parameter
            else {j++;}
        }
        while(i<a.size()){
            score+=3*a[i].second;
            i++;
        }
        return score;
    }
    vector<pair<pair<int,pair<int,int>>,double>> get_score(){
        vector<double> score(nodes.size(),1.0/nodes.size());
        vector<vector<double>> adj_mat=vector<vector<double>>(nodes.size(),vector<double>(nodes.size(),0));
        for(int i=0;i<nodes.size();i++){
            for(int j=0;j<nodes.size();j++){
                adj_mat[i][j]=compare(nodes[j]->words,nodes[i]->words)/(nodes[j]->total_words+1.0);
            }
        }
        for(int i=0;i<nodes.size();i++){
            double sum=0;
            for(int j=0;j<nodes.size();j++){
                sum+=adj_mat[i][j];
            }
            if(sum!=0)
            for(int j=0;j<nodes.size();j++){
                adj_mat[i][j]/=sum;
            }
        }
        for(int i=0;i<10;i++){
            vector<double> temp(nodes.size(),0);
            for(int j=0;j<nodes.size();j++){
                for(int k=0;k<nodes.size();k++){
                    temp[j]+=score[k]*adj_mat[k][j];
                }
            }
            score=temp;
        }
        vector<pair<pair<int,pair<int,int>>,double>> ans;
        for(int i=0;i<nodes.size();i++){
            ans.push_back({{nodes[i]->book_code,{nodes[i]->page,nodes[i]->paragraph}},score[i]});
        }
        return ans;
    }
};

#include <bits/stdc++.h>

void data_analysis(const vector<pair<pair<int,pair<int,int>>,double>>& scores){
    double max_score=0,min_score=INT_MAX,s,sos=0;
    for(auto x:scores){
        max_score=max(max_score,(x.second));
        min_score=min(min_score,(x.second));
        s+=(x.second);
        sos+=(x.second*x.second);
    }
    s/=scores.size();
    sos/=scores.size();
    sos-=s*s;
    cout<<"Number of elements: "<<scores.size()<<endl;
    cout<<"Max score: "<<max_score<<endl;
    cout<<"Min score: "<<min_score<<endl;
    cout<<"Mean score: "<<s<<endl;
    cout<<"Standard deviation: "<<sqrt(sos)<<endl;
    cout<<"Range: "<<max_score-min_score<<endl;
    
}

int choose_k(const vector<pair<pair<int,pair<int,int>>,double>>& scores){
    double sum=0,sum_sq=0,threshold=sqrt(scores[0].second)/1e9,last=0;
    for(int i=0;i<min(10,(int)scores.size());i++){
        sum+=scores[i].second;
        sum_sq+=scores[i].second*scores[i].second;
    }
    last=sum_sq/10-sum*sum/10/10;
    for(int i=10;i<min(20,(int)scores.size());i++){
        sum+=scores[i].second;
        sum_sq+=scores[i].second*scores[i].second;
        // cout<<"std for i="<<i<<" is "<<sum_sq/(i+1)-sum*sum/(i+1)/(i+1)<<endl;
        if((sum_sq/(i+1)-sum*sum/(i+1)/(i+1))-last<0){
            return i;
        }
        last=sum_sq/(i+1)-sum*sum/(i+1)/(i+1);
    }
    return min(20,(int)scores.size());
}

pair<Node*,int> get_analysis(string query,QNA_tool& q){
    vector<pair<string,int>> query_words=rake(query);
    for(auto x:query_words){
        cout<<x.first<<" ";
    }
    cout<<endl;
    int number_of_nodes_per_word=400/(query_words.size()+1);
    Graph g;
    for(auto x:query_words){
        Node* temp=q.get_top_k_para(x.first,5*number_of_nodes_per_word);
		int c=0;
        while(temp&&c<number_of_nodes_per_word){
			if(q.counter->get({temp->book_code,{temp->page,temp->paragraph}})>15){
				g.add_node(temp->book_code,temp->page,temp->paragraph,x,q.counter->get({temp->book_code,{temp->page,temp->paragraph}}));
				c++;
			}
			temp=temp->right;
        }
    }
    vector<pair<pair<int,pair<int,int>>,double>> scores=g.get_score();
    sort(scores.begin(),scores.end(),[](pair<pair<int,pair<int,int>>,double> a,pair<pair<int,pair<int,int>>,double> b){
        return a.second>b.second;
    });
    int k=choose_k(scores);
    Node* head=nullptr;
    for(int i=k-1;i>=0;i--){
        Node* temp=new Node();
        temp->book_code=scores[i].first.first;
        temp->page=scores[i].first.second.first;
        temp->paragraph=scores[i].first.second.second;
        temp->right=head;
        if(head)head->left=temp;
        head=temp;
    }
    return {head,k};
    // data_analysis(scores);
    
}


QNA_tool::QNA_tool(){
    // Implement your function here
	trie=new Tries<pair<int,pair<int,int>>,int>();
    counter=new AVLMap<pair<int,pair<int,int>>,int>();
	extract_csv();
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
	delete trie;
    delete counter;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    string ans="";
    int count=0;
    for (int i=0,l=sentence.size();i<l;i++){
        string x=" .,-:!\"\'()?—[]“”‘’˙;@";
        if(x.find(sentence[i])!=string::npos){
            if(ans.size()!=0)
            {trie->increase_by_1(ans,{book_code,{page,paragraph}},0);count++;}
            ans="";	
        }
    else ans+=sentence[i]>='A'&&sentence[i]<='Z'?(char)(sentence[i]-'A'+'a'):sentence[i];//check complexity and handle upper case number issues
    }
    if(ans.size()!=0)
    {trie->increase_by_1(ans,{book_code,{page,paragraph}},0);count++;}
    counter->increase_by_x({book_code,{page,paragraph}},count);
    return;
}
void QNA_tool::update_avl(AVLMap<pair<int,pair<int,int>>,double>& scores,vector<pair<pair<int,pair<int,int>>,int>>& v,double total){
    for(auto x:v){
        scores.increase_by_x(x.first,x.second*total);
    }
}
Node* QNA_tool::get_top_k_para(string query, int k) {
    string ans="";
        AVLMap<pair<int,pair<int,int>>,double> scores;
      	for (int i=0,l=query.size();i<l;i++){
            string x=" .,-:!\"\'()?—[]“”‘’˙;@";
            if(x.find(query[i])!=string::npos){
                if(ans.size()!=0){
                    Tries<pair<int,pair<int,int>>,int>* temp=trie->get(ans,0);
                    if(temp){
                        vector<pair<pair<int,pair<int,int>>,int>> v;
                        temp->data.get_all(v);
                        update_avl(scores,v,(temp->total+1.0)/(temp->c_val+1.0));
                    }
                }
				ans="";	
            }
	        else ans+=query[i]>='A'&&query[i]<='Z'?(char)(query[i]-'A'+'a'):query[i];//check complexity and handle upper case number issues
        }
        if(ans.size()!=0){
            Tries<pair<int,pair<int,int>>,int>* temp=trie->get(ans,0);
            if(temp){
                vector<pair<pair<int,pair<int,int>>,int>> v;
                temp->data.get_all(v);
                update_avl(scores,v,(temp->total+1.0)/(temp->c_val+1.0));
            }
        }
        vector<pair<pair<int,pair<int,int>>,double>> v;
        scores.get_all(v);
        Heap<pair<double,pair<int,pair<int,int>>>> heap;
        for(auto x:v){
            if(heap.get_size()<k)heap.insert({x.second,x.first});
            else if(heap.get_top().first<x.second){
                heap.pop();
                heap.insert({x.second,x.first});
            }
        }
        Node* head=nullptr;
        while(heap.get_size()){
            Node* temp=new Node();
            temp->book_code=heap.get_top().second.first;
            temp->page=heap.get_top().second.second.first;
            temp->paragraph=heap.get_top().second.second.second;
            temp->right=head;
            if(head)head->left=temp;
            head=temp;
            heap.pop();
        }
        return head;
        
}

void QNA_tool::query(string question, string filename){
    pair<Node*,int> x=get_analysis(question,*this);
    query_llm(filename,x.first,x.second,"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",question);   
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "MKGandhi/mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
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
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}
void QNA_tool::extract_csv(){
	ifstream file("unigram_freq.csv");
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
		trie->insert(word,number,0);
	}
}
void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question){

    // first write the k paragraphs into different files

    Node* traverse = root;
    int num_paragraph = 0;

    while(num_paragraph < k){
        assert(traverse != nullptr);
        string p_file = "paragraph_";
        p_file += to_string(num_paragraph);
        p_file += ".txt";
        // delete the file if it exists
        remove(p_file.c_str());
        ofstream outfile(p_file);
        string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
        assert(paragraph != "$I$N$V$A$L$I$D$");
        outfile << paragraph;
        outfile.close();
        traverse = traverse->right;
        num_paragraph++;
    }

    // write the query to query.txt
    ofstream outfile("query.txt");
    outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
    outfile << question;
    // You can add anything here - show all your creativity and skills of using ChatGPT
    outfile.close();
 
    // you do not need to necessarily provide k paragraphs - can configure yourself

    // python3 <filename> API_KEY num_paragraphs query.txt
    string command = "python ";
    command += filename;
    command += " ";
    command += API_KEY;
    command += " ";
    command += to_string(k);
    command += " ";
    command += "query.txt";

    system(command.c_str());
    return;
}
