#include <bits/stdc++.h>
using namespace std;

#define mp make_pair
#define pb push_back
#define db(x) cerr << #x << " == " << x << endl;
#define _ << " " <<
#define st first
#define nd second

typedef long long ll;
typedef vector<int> vi;
typedef pair<int,int> ii;

const long double EPS = 1e-9;
const int N=1e5+5;
const int MOD=1e9+7;
const int INF=0x3f3f3f3f;

int col, lines, maxval;
vector<vector<int>> grid;
unordered_map<int,int> freq;
unordered_map<int,string> code;
ll total_freq;

//reads image.pgm file and builds the pixel values grid
void readImage() {
	fstream image("lena_ascii.pgm");
	string s;
	int pixel;
	if(image.is_open()) {
		getline(image,s); //P2
		getline(image,s); //comment
		image >> col >> lines >> maxval;
		for(int i=0;i<lines;i++) {
			vi gridLine;
			for(int j=0;j<col;j++) {
				image >> pixel;
				gridLine.pb(pixel);
			}
			grid.pb(gridLine);
		}
	}
	image.close();
}

//find the frequency for each different pixel value stored in the grid 
void findFrequency() {
	for(int i=0;i<lines;i++) {
		for(int j=0;j<col;j++) {
			if(freq.count(grid[i][j]) > 0) freq[grid[i][j]]++;
			else freq[grid[i][j]] =1;
		}
	}
}

struct Node{
	int freq;
	int pixel;
	Node *left, *right;
	Node(){
		left=right= nullptr;
		pixel = -1;
	}
};

//heap which stored the one node and its frequency
priority_queue <pair<int, Node*>> heap;

//fills the heap with the known nodes until now (future leafs of the tree)
void fillHeap() {
	for(auto element : freq) {
		Node *aux = new Node();
		aux -> pixel = element.st;
		aux -> freq = element.nd;
		heap.push(mp(-element.nd, aux));
		total_freq += element.nd;
	}
}

//builds the tree by removing elements from the heap and joining 2 nodes with smallest frequency values
Node* buildTree() {
	findFrequency();
	fillHeap();
	while(heap.size()>1) {
		pair<int,Node*> u,v;
		u=heap.top();
		heap.pop();
		v=heap.top();
		heap.pop();
		Node* aux= new Node();
		aux-> freq= -u.st-v.st;
		aux->left= u.nd;
		aux->right=v.nd;
		heap.push(mp(u.st+v.st,aux));
	}
	return heap.top().nd; //root of the tree
}

//generate the binary code words for each pixel value in the tree (each leaf will have one code word)
void generateCodeWords(Node *root,string s){
	if(root==nullptr) return;
	if(root->pixel!=-1){
 		code[root->pixel]=s;
 		return;
 	}
 	generateCodeWords(root->left, s+'0');
 	generateCodeWords(root->right,s+'1');
}

//writes the binary huffman codes in a file called lena_ascii.huff
void writeHuffmanCodes() {
	fstream huffman("lena_ascii.huff");
	if(huffman.is_open()) {
		huffman << col << " " << lines << " " << maxval << " " << code.size() << endl;
		for(auto x : code) {
			huffman << x.st << " " << x.nd << " ";
		}
		huffman << endl;
		for(int i=0;i<lines;i++) {
			for(int j=0;j<col;j++) {
				huffman << code[grid[i][j]];
			}
		}
		huffman << endl;
	}
	huffman.close();
}

//finds the medium length of a code word
double findMediumLengthCodeWord() {
	ll sum = 0;
	for(auto x : code) {
		sum += x.nd.size()*freq[x.st];
	}
	double ans = (double)sum/total_freq;
	return ans;
}

int main(){
	readImage();
	Node* root = new Node();
	root = buildTree();
	generateCodeWords(root,"");
	writeHuffmanCodes();
	cout << "O comprimento medio do codigo binario de Huffman vale " <<	findMediumLengthCodeWord() << " bits." << endl;
}