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
    
    if(searchBuffer[index+1] != lookAhead[lookPoint+1]){
            matchCont = false;
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

        for(int i = 0; i < match.size(); i++){

//            cout << match[i];

        }

//        cout << endl;

        return (int)match.size();
        
    }else{
        
        return 0;
    }
    

}

void fillSearchBuff(int searchBufferLength, int lookAheadLength, int matchL){
    
    
    if(lookAhead.empty()){
        
        for(int i = 0; i < searchBufferLength; i++){
//
            searchBuffer.push_back('0');
//            cout << searchBuffer[i];
//
        }
//
//        cout << endl;
        
    }else{
        
        if(matchL == 0 || matchL == 1){
            
            searchBuffer.erase(searchBuffer.begin());
            searchBuffer.push_back(lookAhead[0]);
            
        }else if(matchL > 1){
            
            for(int i = 0; i < matchL; i++){
                
                searchBuffer.erase(searchBuffer.begin());
                searchBuffer.push_back(lookAhead[i]);
                
            }
            
        }
        
        
//        for(int i = 0; i < searchBufferLength; i++){
//
//            cout << searchBuffer[i];
//        }
//        cout << endl;
        
        
    }
    
}

//fill look ahead buffer

void fillLookAhead(int lookAheadLength, vector<unsigned char> text, int &pointer, int matchL){
    
    if(!lookAhead.empty()){
        
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

void estimatedCompression(vector <int> lengthBuffer){
    
    int newLength = ceil((double)lengthBuffer.size()/2);
    int totalFirst = 0;
    int totalSecond = 0;

    cout << "Estimated Compression: " << endl;
    cout << "Total of first half: ";
    for(int i = 0; i < newLength; i++){
        
        totalFirst += lengthBuffer[i];
        
    }
    
    cout << totalFirst << endl;
    
    for(int i = 0; i < newLength; i++){
        
        cout << i << " : " << lengthBuffer[i] << " / " << totalFirst << " = " << ((double)lengthBuffer[i]/totalFirst) << endl;

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

int calcBitsSent(int bitlength1, int bitlength2){
    
    int sumTotal = 0;
    int storeLog = log10(bitlength1)/log10(2);
    int storeLog2 = log10(bitlength2)/log10(2);
    sumTotal = storeLog + storeLog2;
    
    sumTotal++;
    
    return sumTotal;
    
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
        
        if(i == 0){
            
            bitsSent += lengthBuffer[i] * 9;
            bytesInFile += lengthBuffer[i];
            
        }else if (i==1){
            
            bitsSent += lengthBuffer[i] * bits;
            bytesInFile += lengthBuffer[i];
            
        }else{
            
            bitsSent += lengthBuffer[i] * bits;
            bytesInFile += lengthBuffer[i] * i;

        }
        
    }
    
    bitsInFile = bytesInFile*8;
    
    cout << "Total Bytes: " << bytesInFile << endl;
    cout << "Total Bits: " << bitsInFile << endl;
    cout << "Bits sent: " << bitsSent << endl;
    cout << "Compression (bitsInFile/bitsSent): " << (double)(bitsInFile/bitsSent)<< endl;
    cout << "Alternative Compression (1/Compression): " << (double)(bitsSent/bitsInFile) << endl;
    estimatedCompression(lengthBuffer);
    
}


void compress(int searchBufferLength, int lookAheadLength, vector<unsigned char> text){
    
    vector<int> lengthBuffer(lookAheadLength+1, 0);
    vector <int> indexBuffer(lookAheadLength, 0);

    int matchL = 0;
    
    for(int pointer = 0; pointer < text.size()+lookAheadLength-1; pointer++){
    
//        cout << "Search buffer: ";
        fillSearchBuff(searchBufferLength, lookAheadLength, matchL);
//        cout << "Look ahead buffer: ";
        fillLookAhead(lookAheadLength, text, pointer, matchL);
        matchL = match(searchBufferLength, lookAheadLength, pointer, indexBuffer);
        storeLengthBuffer(lengthBuffer, matchL);
//        displayLengthBuffer(lengthBuffer, lookAheadLength);
        
    }
    cout << endl;
    stats(text, lengthBuffer, searchBufferLength, lookAheadLength);
    displayLengthBuffer(lengthBuffer, lookAheadLength);
    
}

int main(int argc, const char * argv[]) {
    
    cout << "LZSS Program" << endl;
//    cout << endl;
    
    ifstream file("/Users/valeriajara/Desktop/LZSS/LZSSTest/pic", ifstream::binary);
    
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
    
    int searchBufferLength = 512;
    int lookAheadLength = 256;
    
    compress(searchBufferLength, lookAheadLength, text);
    
}
