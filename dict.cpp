// Do NOT add any other includes
#include "dict.h"
string w_t_l(string x){
	string ans="";
	for(int i=0;i<x.size();i++)ans+=x[i]>='A'&&x[i]<='Z'?(char)(x[i]-'A'+'a'):x[i];
	return ans;
}
TrieNode::TrieNode():children(new AVLTree()),word(""),word_count(0),pres_word(""){}
TrieNode::TrieNode(string word):children(new AVLTree()),word(w_t_l(word)),word_count(0),pres_word(""){}
TrieNode::TrieNode(string word,string past):children(new AVLTree()),word(w_t_l(word)),word_count(0),pres_word(past){}
TrieNode* TrieNode::get_child(char c){
		if(c<='Z'&&c>='A')c-=('A'-'a');
		return children->find(c);
}
void TrieNode::set_child(char c,TrieNode* i){
		c=tolower(c);
		children->insert(i,c);
}
//AVL tree implementation
int AVLTree::height(AVLnode* i){
	if(!i)return -1;
	return i->height;
}
AVLnode* AVLTree::rotate_left(AVLnode* i){
	AVLnode* j=i->left;
	i->left=j->right;
	j->right=i;
	i->height=max(height(i->left),height(i->right))+1;
	j->height=max(height(j->left),height(j->right))+1;
	return j;
}
AVLnode* AVLTree::rotate_right(AVLnode* i){
	AVLnode* j=i->right;
	i->right=j->left;
	j->left=i;
	i->height=max(height(i->left),height(i->right))+1;
	j->height=max(height(j->left),height(j->right))+1;
	return j;
}
AVLnode* AVLTree::recur(TrieNode* i,char c,AVLnode* r){
	c=tolower(c);
	if(!r){
		return new AVLnode(c,i);
	}
	if(r->c>c)r->left=recur(i,c,r->left);
	else if(r->c<c)r->right=recur(i,c,r->right);
	else r->next=i;//TODO:check memorey leaks
	int l=height(r->left),rr=height(r->right);
	r->height=max(l,rr)+1;
	if(abs(l-rr)>1){
		if(l>rr){
			if(height(r->left->left)>height(r->left->right))r=rotate_left(r);
			else{
				r->left=rotate_right(r->left);
				r=rotate_left(r);
			}
		}
		else{
			if(height(r->right->right)>height(r->right->left))r=rotate_right(r);
			else{
				r->right=rotate_left(r->right);
				r=rotate_right(r);
			}
		}
	}
	return r;
}
void AVLTree::insert(TrieNode* i,char c){
	c=tolower(c);
	root=recur(i,c,root);
}
TrieNode* AVLTree::find(char c){
	c=tolower(c);
	AVLnode* i=root;
	while(i){
		if(i->c>c)i=i->left;
		else if(i->c<c)i=i->right;
		else return i->next;
	}
	return nullptr;
}
void AVLTree::addALL(vector<TrieNode*>& st,AVLnode* i){
	if(!i)return;
	if(i->next)st.push_back(i->next);
	addALL(st,i->left);
	addALL(st,i->right);
}
void AVLTree::addALL(vector<TrieNode*>& st){
	addALL(st,root);
}

//Trie implementation,Note this is a radix trie
Trie::Trie(){
	root=new TrieNode();
}
void Trie::insert(string word){
    if(word.size()==0)return;
	if(!root->get_child(word[0])){
		root->set_child(word[0],new TrieNode(word,word));
		root->get_child(word[0])->par=root;
		root->get_child(word[0])->word_count++;
		return;
	}
	int c=0,l=word.size();
	TrieNode* i=root->get_child(word[c]);
	while(c<l){
		int j=0;
		for(j=0;c<l&&j<i->word.size()&&word[c]==i->word[j];j++,c++);
		if(j<i->word.size()){//improve complexity of substr
			TrieNode* temp=new TrieNode(i->word.substr(0,j),i->par->pres_word+i->word.substr(0,j));
			temp->par=i->par;
            temp->par->set_child(i->word[0],temp);
            i->word=i->word.substr(j);
			temp->set_child(i->word[0],i);
			i->par=temp;
			i=temp;
		}
		if(c==l)break;
		if(!i->get_child(word[c])){i->set_child(word[c],new TrieNode(word.substr(c),word));i->get_child(word[c])->par=i;i=i->get_child(word[c]);break;}
		else i=i->get_child(word[c]);
	}
	i->word_count++;
}
int Trie::get_count(string word){
	int c=0;TrieNode* i=root;
	while(c<word.size()){
        if(!i)return 0;
		for(int j=0;j<i->word.size();j++,c++)if(c==word.size()||word[c]!=i->word[j])return 0;
        if(c==word.size())break;
		i=i->get_child(word[c]);
	}
    if(!i)return 0;
	return i->word_count;
}
void Trie::write_to_file(string filename){
    fstream file;
    file.open(filename,ios::out);
    if(!file.is_open())return;
    vector<TrieNode*> stack;
    stack.push_back(root);
    while(!stack.empty()){
        TrieNode* i=stack.back();
        stack.pop_back();
        if(i->word_count)file<<i->pres_word<<", "<<i->word_count<<endl;
        // for(int j=0;j<36;j++)if(i->children[j])stack.push_back(i->children[j]);
		i->children->addALL(stack);
    }
    file.close();
}
Trie::~Trie(){
	delete root;
}

//Dict Implementation
Dict::Dict(){
    // Implement your function here    
    t=new Trie();
}

Dict::~Dict(){
    // Implement your function here
    delete t;    
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here
    	string ans="";
      	for (int i=0,l=sentence.size();i<l;i++){
            string x=" .,-:!\"\'()?—[]“”‘’˙;@";
            if(x.find(sentence[i])!=string::npos){
                if(ans.size()!=0)
				t->insert(ans);
				ans="";	
            }
	    else ans+=sentence[i]>='A'&&sentence[i]<='Z'?(char)(sentence[i]-'A'+'a'):sentence[i];//check complexity and handle upper case number issues
        }
	if(ans.size()!=0)
	t->insert(ans);
    return;
}

int Dict::get_word_count(string word){
    // Implement your function here  
 	word=w_t_l(word);
    return t->get_count(word);
}

void Dict::dump_dictionary(string filename){
    // Implement your function here
   	t->write_to_file(filename);  
    return;
}
