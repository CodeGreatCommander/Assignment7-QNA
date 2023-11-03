// Do NOT add any other includes
#include "search.h"


//SearchEngine Implementation
SearchEngine::SearchEngine(){
    // Implement your function here
}

SearchEngine::~SearchEngine(){
	// Implement your function here 
}
long long int SearchEngine::hash(string s){
	long long int ans=0;
	for (int i=0,l=s.size();i<l;i++){
		ans=(ans*seed+(s[i]<='Z'&&s[i]>='A'?s[i]-'A'+'a':s[i]))%mod;
	}
	return ans;
}
long long int SearchEngine::power(int a,int b){
	if(b==0)return 1;
	long long int ans=power(a,b/2);
	ans=(ans*ans)%mod;
	if(b%2==1)ans=(ans*a)%mod;
	return ans;
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    // Implement your function here  
    this->sentence.push_back(sentence);
	this->book_code.push_back(book_code);
	this->page.push_back(page);
	this->paragraph.push_back(paragraph);
	this->sentence_no.push_back(sentence_no);
	this->position.push_back(sentence.size());
}
char SearchEngine::conv(char a){
	if(a<='Z'&&a>='A')return a-'A'+'a';
	return a;
}
Node* SearchEngine::search(string pattern, int& n_matches){
	n_matches=0;
   	long long int hash_pattern=hash(pattern);
	int l=pattern.size();
	Node* root=nullptr;
	for(int i=sentence.size()-1;i>=0;i--){
		long long int hash_comp=hash(sentence[i].substr(0,l));
		for(int j=l-1;j<=sentence[i].size();j++){
			if(hash_comp==hash_pattern){
				bool flag=true;
				for(int k=0;k<l;k++){
					if(conv(sentence[i][j-l+k+1])!=conv(pattern[k])){
						flag=false;
						break;
					}
				}
				if(flag){
					n_matches++;
					Node* temp=new Node(book_code[i],page[i],paragraph[i],sentence_no[i],j-l+1);
					temp->left=nullptr;
					temp->right=root;
					if(root!=nullptr)root->left=temp;
					root=temp;
				}
			}
			if(j==sentence[i].size())break;
			hash_comp=(hash_comp*seed+conv(sentence[i][j+1])-power(seed,l)*conv(sentence[i][j-l+1]))%mod;
			if(hash_comp<0)hash_comp+=mod;
		}
	}
	return root;
}
