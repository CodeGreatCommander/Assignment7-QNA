#include "../Node.cpp"
using namespace std;
//LinkList Implementation
class LinkList{
private:
	Node* start;
	int size;
public:
	LinkList():start(nullptr),size(0){}
	void insert(int book_code,int page,int paragraph,int sentence_no,int offset=0){
		Node* temp=new Node(book_code,page,paragraph,sentence_no,offset);
		temp->left=start;
		start->right=temp;
		start=temp;
	}
	int get_size(){
		return size;
	}
	Node* get_start(){
		return start;
	}
	~LinkList(){
		Node* i=start;
		while(i){
			Node* temp=i->right;
			delete i;
			i=temp;
		}
	}
};
//AVLMap implementation
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
		if(r->d==d)return r;
		else if(r->d<d)return find(d,r->right);
		else return find(d,r->left);
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
	X get(T d){

		Node* x=find(d,root);
		if(!x)return NULL;
		else return x->v;
	}
};


//Trie Implementation
template <class t,class d>
class Tries{
public:
	AVLMap<char,Tries*> child;
	AVLMap<t,d> data;
	int c_val;
	void insert(string x,int c_val,int i){//initially pass parameter 0
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
	Tries* get(string x,int i=0){
		if(i==x.length()){return this;}
		Tries* f=child.get(x[i]);
		if(!f){
			return nullptr;
		} 
		return f->get(x,i+1);
	}
};


//Heap Implementation
#include <vector>
template <class T>
class Heap{
	vector<T> heap;
	size_t size;
	size_t par(size_t i){
		return (i-1)/2;
	}
	void swap(T& a,T&b){
		T& c=a;
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
		if(size==0){heap.push_back(x);return;size++;}
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
					swap(heap[left(i)]<heap[right(i)]);
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
		if(size==0)return NULL;
		return heap[0];
	}
};

