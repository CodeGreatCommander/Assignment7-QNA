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


QNA_tool::QNA_tool(){
    // Implement your function here
	trie=new Tries<pair<int,pair<int,int>>,int>();
	extract_csv();
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
	delete trie;
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    string ans="";
    for (int i=0,l=sentence.size();i<l;i++){
        string x=" .,-:!\"\'()?—[]“”‘’˙;@";
        if(x.find(sentence[i])!=string::npos){
            if(ans.size()!=0)
            trie->increase_by_1(ans,{book_code,{page,paragraph}},0);
            ans="";	
        }
    else ans+=sentence[i]>='A'&&sentence[i]<='Z'?(char)(sentence[i]-'A'+'a'):sentence[i];//check complexity and handle upper case number issues
    }
    if(ans.size()!=0)
    trie->increase_by_1(ans,{book_code,{page,paragraph}},0);
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
		cout<<v.size()<<" "<<k<<endl;
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
    // Implement your function here  
    std::cout << "Q: " << question << std::endl;
    std::cout << "A: " << "Studying COL106 :)" << std::endl;
    return;
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
    string command = "python3 ";
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