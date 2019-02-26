//
//  main.cpp
//  TheLZSSAlgorithm
//
//  Created by Valeria Jara on 1/21/19.
//  Copyright © 2019 Valeria Jara. All rights reserved.
//

#include <sstream>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <list>
#include <iterator>
#include <math.h>
using namespace std;

//fill search buffer

vector <unsigned char> searchBuffer;
vector <unsigned char> lookAhead;

int match(int searchBufferLength, int lookAheadLength, int pointer, vector <int> &indexBuffer){
	
	vector <unsigned char> match;
	bool matchCont = true;
	int lookPoint = 0;
	long int index = 0;
    
	auto it = find(searchBuffer.begin(), searchBuffer.end(), lookAhead[0]);
	
	if (it != searchBuffer.end()){
		
		match.push_back(lookAhead[lookPoint]);
		index = distance(searchBuffer.begin(), it);
//        cout << "MATCH IS " << lookAhead[lookPoint] << " AND INDEX IS " << searchBufferLength-index-1 << endl;

	}
    if(!searchBuffer.empty()){
        
        if(searchBuffer[index+1] != lookAhead[lookPoint+1]){
            matchCont = false;
        }
        
        
    }
	while(matchCont == true && !match.empty() && match.size() < lookAheadLength){
		
		index++;
		lookPoint++;

		if(searchBuffer[index] != lookAhead[lookPoint]){
			matchCont = false;
			
		}else{
		
//        cout << "MATCH IS " << lookAhead[lookPoint] << " AND INDEX IS " << searchBufferLength-index-1 << endl;
			
		match.push_back(lookAhead[lookPoint]);
			
		}

	}
	
	if(!match.empty()){
		
//        cout << "THE MATCH IS:"<< endl;
//
//        for(int i = 0; i < match.size(); i++){
//
//            cout << match[i];
//
//        }
//
//        cout << endl;

		return (int)match.size();
		
	}else{
		
		return 0;
	}
	

}

void fillSearchBuff(int searchBufferLength, int lookAheadLength, int matchL){
	
	
	if(lookAhead.empty()){
		
//        for(int i = 0; i < searchBufferLength; i++){
////
//            searchBuffer.push_back('0');
////            cout << searchBuffer[i];
////
//        }
////
		
	}else{
		
		if(matchL == 0 || matchL == 1){
			
            if(searchBuffer.size() == searchBufferLength){
                searchBuffer.erase(searchBuffer.begin());
            }
			searchBuffer.push_back(lookAhead[0]);
			
		}else if(matchL > 1){
			
            if(!searchBuffer.empty() && searchBuffer.size() < searchBufferLength){

                for(int i = 0; i < matchL; i++){
                    
                    searchBuffer.push_back(lookAhead[i]);
                    
                }
                
            }else{
                
                for(int i = 0; i < matchL; i++){
                    
                    searchBuffer.erase(searchBuffer.begin());
                    searchBuffer.push_back(lookAhead[i]);
                    
                }
                
            }
		}
		
        
//        for(int i = 0; i < searchBuffer.size(); i++){
//
//            cout << searchBuffer[i];
//        }
//        cout << endl;
        
		
	}
	
}


int calcBitsSent(int bitlength1, int bitlength2){
    
    int sumTotal = 0;
    int storeLog = log10(bitlength1)/log10(2);
    int storeLog2 = log10(bitlength2)/log10(2);
    sumTotal = storeLog + storeLog2;
    
    sumTotal++;
    
    return sumTotal;
    
}

//fill look ahead buffer

void fillLookAhead(int lookAheadLength, vector<unsigned char> &text, int &pointer, int matchL){
    
    
	if(!lookAhead.empty()){
		
//        cout << endl;
//
		if(matchL == 0 || matchL == 1){
			
            lookAhead.erase(lookAhead.begin());
            lookAhead.push_back(text[pointer]);
            
            pointer++;

			
			
		}else if(matchL > 1){
			
			for(int i = 0; i < matchL; i++){
				
                lookAhead.erase(lookAhead.begin());
                lookAhead.push_back(text[pointer]);

                pointer++;

				
			}
			
		}
		
	
	}else{
		
		for(int i = 0; i < lookAheadLength; i++){

            lookAhead.push_back(text[pointer]);
                
//                cout << endl;
//                cout <<"text " << text[pointer] << endl;
//

            pointer++;

			
		}

		
	}
	
//    for(int i = 0; i < lookAheadLength; i++){
//
//        cout << lookAhead[i];
//
//    }
//
//    cout << endl;
	pointer--;
	
}

void estimatedCompression(vector <int> &lengthBuffer, int searchBufferLength, int lookAheadLength, int newLook, int staticSearch){
	
	int newLength = ceil((double)lengthBuffer.size()/2);
    double kBuffer[newLength];
	int totalFirst = 0;
	int totalSecond = 0;

	cout << "Estimated Compression: " << endl;
    
	for(int i = 0; i < newLength; i++){
		
		totalFirst += lengthBuffer[i];
		
	}
    
    for(int i = newLength; i < lengthBuffer.size(); i++){
        
        totalSecond += lengthBuffer[i];
        
    }

    cout << "Total of first half: ";
	cout << totalFirst << endl;
	
	for(int i = 0; i < newLength; i++){
		
		cout << i << " : " << lengthBuffer[i] << " / " << totalFirst << " = " << ((double)lengthBuffer[i]/totalFirst) << endl;
        kBuffer[i] = ((double)lengthBuffer[i]/totalFirst);

	}
    
    cout << endl;
    cout << "Total of second half: ";
    cout << totalSecond << endl;
    cout << "K is equal to: " << endl;
    for(int i = 0; i < newLength; i++){
        
        kBuffer[i] = round(kBuffer[i]*totalSecond);
        cout << i << " : " << kBuffer[i] << endl;
        
    }
    
    cout << endl;
    cout << "New Histogram: " << endl;
    long int bits = calcBitsSent(searchBufferLength, ceil((double)(lookAheadLength+1)/2));
    long double totalSentBits = 0;
    int newHist[newLength];
    long double newTotal = 0;
    
    for(int i = 0; i < newLength; i++){
        
        if(i*9 <= bits){
            newHist[i] = lengthBuffer[i] + kBuffer[i];
            totalSentBits += newHist[i]*9;
            cout << i << ": " << newHist[i] << " * 9 = " << newHist[i] * 9 << endl;
            
        }else{
            newHist[i] = lengthBuffer[i] + kBuffer[i];
            totalSentBits += newHist[i]*bits;
            cout << i << ": " << newHist[i] << " * " << bits << " = " << newHist[i]*bits << endl;
            
        }
        
        newTotal += newHist[i];
    }
    
    cout << endl;
    cout << totalSentBits << endl;
    cout << "Total bytes (Added values of new histogram): " << newTotal << endl;
    cout << "Total bits: (Added values of new histogram): " << newTotal*8 << endl;
    cout << "New Sent bits: (Sending 9 bits for misses and certain matches, sending " << bits << " bits per match): " << totalSentBits << endl;
    cout << "Estimated Compression Ratio: (Total Bits / New Sent bits) "<< (double)((double)(newTotal*8)/(double)totalSentBits)<< endl;
    cout << "This is " << searchBufferLength << "x" << newLook << endl;
    
    newLook = ceil((double)(lookAheadLength+1)/2)-1;
    vector<int> newHistVec(newHist, newHist + newLength);

    if(newLook/2 > 1){

        cout << "---------" << endl;

        estimatedCompression(newHistVec, staticSearch/2, lookAheadLength/2, newLook/2, staticSearch);

    }
    
}

void storeLengthBuffer(vector <int> &lengthBuffer, int matchL){
	
	lengthBuffer[matchL]++;
	
}

void displayLengthBuffer(vector <int> &lengthBuffer, int lookAheadLength){
	
	cout << "Length Buffer: " << endl;
	
	for(int i = 0 ; i < lookAheadLength + 1; i++){
		
		cout << lengthBuffer[i] << " ";
		
	}
	cout << endl;
	
}

void stats(vector<unsigned char> text, vector<int> lengthBuffer, int searchBufferLength, int lookAheadLength){
   
	double bytesInFile = 0;
	double bitsInFile = 0;
	long int bitsSent = 0;
	long int bits = calcBitsSent(searchBufferLength,  lookAheadLength);
	
	cout << "Bits: " << bits << endl;
	cout << "Total Bytes: (From text.size()): " << text.size() << endl;
	cout << "Total Bits: (From text.size()): " << text.size()*8 << endl;
	
	for(int i = 0; i < lengthBuffer.size(); i++){
		
//        bytesInFile += lengthBuffer[i];
        
        if(i == 0){
            
            bytesInFile += lengthBuffer[i];
            
        }else{
            
            bytesInFile += lengthBuffer[i]*i;

        }

        

		if(i*9 <= bits){
            
			bitsSent += lengthBuffer[i] * 9;
//        }else if (i==1){
//
//            bitsSent += lengthBuffer[i] * bits;
//            bytesInFile += lengthBuffer[i];
		}else{
			bitsSent += lengthBuffer[i] * bits;
		}
		
	}
	
	bitsInFile = bytesInFile*8;
	
	cout << "Total Bytes: " << bytesInFile << endl;
	cout << "Total Bits: " << bitsInFile << endl;
	cout << "Bits sent: " << bitsSent << endl;
	cout << "Compression (bitsInFile/bitsSent): " << (double)(bitsInFile/bitsSent)<< endl;
	cout << "Alternative Compression (1/Compression): " << (double)(bitsSent/bitsInFile) << endl;
    cout << endl;
    int newLook = ceil((double)(lookAheadLength+1)/2)-1;
    int countSearchBuffer = searchBufferLength;

    while(lookAheadLength > 2){
        
        estimatedCompression(lengthBuffer, searchBufferLength, lookAheadLength, newLook, countSearchBuffer);
        
        cout << "---------" << endl;
        
        countSearchBuffer = countSearchBuffer/2;
        lookAheadLength = lookAheadLength/2;
        newLook = newLook/2;
    }
	
}

void compress(int searchBufferLength, int lookAheadLength, vector<unsigned char> text){
	
	vector<int> lengthBuffer(lookAheadLength+1, 0);
	vector <int> indexBuffer(lookAheadLength, 0);
	int matchL = 0;
    
	
	for(int pointer = 0; pointer < text.size()+lookAheadLength-1; pointer++){
    
//            cout << "Search buffer: ";
            fillSearchBuff(searchBufferLength, lookAheadLength, matchL);
//            cout << "Look ahead buffer: ";
            fillLookAhead(lookAheadLength, text, pointer, matchL);
            matchL = match(searchBufferLength, lookAheadLength, pointer, indexBuffer);
            storeLengthBuffer(lengthBuffer, matchL);
//            displayLengthBuffer(lengthBuffer, lookAheadLength);

    }
	cout << endl;
    displayLengthBuffer(lengthBuffer, lookAheadLength);
	stats(text, lengthBuffer, searchBufferLength, lookAheadLength);
	
}

int main(int argc, const char * argv[]) {
	
	cout << "LZSS Program" << endl;
//    cout << endl;
	
	ifstream file("/Users/valeriajara/Desktop/LZSS/LZSSTest/bib", ifstream::binary);
	
	vector<unsigned char> text;
	
	//read file
	if(file){
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0,file.beg);
		
		char * c = new char[1];
		
		for(int i = 0; i < length; i++){
			file.read(c,1);
			text.push_back(*c);
		}
		
		if(!file){
			file.close();
		}
		
	}
	
	int searchBufferLength = 128;
    int lookAheadLength = 64;
    
	
	compress(searchBufferLength, lookAheadLength, text);
	
}
