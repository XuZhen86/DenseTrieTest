#include<iostream>
#include<map>
#include<vector>
#include<string>
#include<fstream>
#include<cstdlib>
#include<chrono>
#include<random>
using namespace std;

#include<../DenseTrie/densetrie.h>

int main(int argc,char *argv[]){
    if(argc!=3){
        cout<<"Usage: "<<argv[0]<<" wordsFileName trialTimes"<<endl;
        return 1;
    }

    long long trialTimes=atoll(argv[2]);
    if(!trialTimes){
        cout<<"Invalid trialTimes"<<endl;
        return 2;
    }

    fstream f(argv[1],fstream::in);
    if(!f.is_open()){
        cout<<"Failure when opening file: "<<argv[1]<<endl;
        return 3;
    }

    int startClock=0;
    map<string,bool> wordsMap;
    vector<string> words;
    DenseTrie d;

    cout<<"Started inserting words,  clock()="<<(startClock=clock())<<endl;
    while(!f.eof()){
        string str;
        getline(f,str);

        if(str.empty()){
            continue;
        }

        words.push_back(str);
        wordsMap[str]=true;
        d.insert(str.c_str());
    }
    f.close();
    cout<<"Finished inserting words, clock()="<<clock()<<", elapsed="<<(clock()-startClock)<<endl;

    cout<<"Started consolidate(),    clock()="<<(startClock=clock())<<endl;
    d.consolidate();
    cout<<"Finished consolidate(),   clock()="<<clock()<<", elapsed="<<(clock()-startClock)<<endl;

    random_device randDev;

    cout<<"Started testing,          clock()="<<(startClock=clock())<<endl;
    while(trialTimes--){
        const string &str=words.at(randDev()%words.size());

        switch(randDev()%3){
            case 0: // Test validity
                if(wordsMap[str]!=d.contains(str.c_str())){
                    cout<<"Error: str="<<str<<" wordsMap="<<wordsMap[str]<<" d.contains()="<<d.contains(str.c_str())<<endl;
                }
                break;
            case 1: // Disable
                wordsMap[str]=false;
                d.disable(str.c_str());
                break;
            case 2: // Enable
                wordsMap[str]=true;
                d.enable(str.c_str());
                break;
        }
    }
    cout<<"Finished testing,         clock()="<<clock()<<", elapsed="<<(clock()-startClock)<<endl;
}
