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

int col, lines, maxval, size, pixel;
string word, binaryText;
vi grid;
unordered_map<int,string> code;
unordered_map<string,int> decode;

//reads the file called lena_ascii.huff and gets the binary sequence written there (binaryText)
void readHuffmanCodes() {
	fstream huffman("lena_ascii.huff");
	if(huffman.is_open()) {
		huffman >> col >> lines >> maxval >> size;
		for(int i=0;i<size;i++) {
			huffman >> pixel >> word;
			code[pixel] = word;
			decode[word] = pixel;
		}
		huffman >> binaryText;
	}
	huffman.close();
}

//reads the binary sequence and decodes it, to recover the original pixel values (grid)
void recoverPixelGrid() {
	string current = "";
	for(char c : binaryText) {
		current += c;
		if(decode.count(current) > 0) {
			grid.pb(decode[current]);
			current="";
		}
	}
}

//writes in a file called lena_ascii.huff.pgm the data needed to generate a pgm image
void generateImage() {
	fstream image("lena_ascii.huff.pgm");
	int k=0;
	if(image.is_open()) {
		image << "P2" << endl;
		image << col << " " << lines << " " << maxval << endl;
		for(int i=0;i<lines;i++) {
			for(int j=0;j<col;j++) {
				image << grid[k] << " ";
				k++;
			}
			image << endl;
		}

	}
	image.close();
}

/* CODE TO COMPUTE PSNR BETWEEN 2 IMAGES */
 
 vi original_grid; //vector with pixel values from original image

//reads original image.pgm file and builds the pixel values original_grid
void readOriginalImage() {
	fstream original("lena_ascii.pgm");
	string s;
	int pixel;
	if(original.is_open()) {
		getline(original,s); //P2
		getline(original,s); //comment
		original >> col >> lines >> maxval;
		for(int i=0;i<lines;i++) {
			for(int j=0;j<col;j++) {
				original >> pixel;
				original_grid.pb(pixel);
			}
		}
	}
	original.close();
}

//Calculates image PSNR value
double imagePSNR() {  
	readOriginalImage();
    double MSE=0.0;
    double MSEcurrent=0.0;
    double psnr=0.0;
    double val;
    int MAX = maxval*maxval;
 
    //Calculate MSE
    for(int i=0;i<lines*col;i++)
    {
        MSEcurrent=abs(original_grid[i]-grid[i]);
        MSE+=MSEcurrent*MSEcurrent;
    }
    MSE/=col*lines;
 
    //Detail here: if MSE is zero, val will be inf! This means there was no loss in the compression
    val = (double)MAX/MSE;
    
    psnr=10*log10(val);
    return psnr;
}

int main(){
	readHuffmanCodes();
	recoverPixelGrid();
	generateImage();
	cout << "A PSNR entre as imagens vale " << imagePSNR() << endl;
}