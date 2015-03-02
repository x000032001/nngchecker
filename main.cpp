#include <iostream>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include "inputer.h"
#define ERR -1
using namespace std;

int conv( string str )
{
    if( str[0] != '$' )
        return ERR;

    int probNum = 0;
    for( int i = 1 ; i < str.size() ; ++i )
    {
        if( '0' <= str[i] && str[i] <= '9' )
        {
            probNum *= 10;
            probNum += str[i] -'0' ;
        }
    }

    return probNum;
}

int main(int argc, char *argv[])
{
    int verb=0;
    if( argc==2 && !strcmp(argv[1],"-v") )
    {
        verb=1;
    }

    inputer input;
    fstream fs("output.txt", std::fstream::in );
    if( fs.fail() )
    {
        cout << "output file not found (output.txt)" << endl;
        cout << "Press any key to exit" << endl;
        fgetc(stdin);
    }
    string line;
    int count=0;
    int maxp=0;
    vector<int> wrongp;
    vector<int> emptyp;
	set<int> solved;

    int pre = 0;
    while( 1 )
    {
        getline(fs,line);
        int probNum=0;
        if( line.size() <= 1 )
            goto DONE;
        if( ERR == (probNum = conv(line)) ) 
        {
            cout << "GET trash line at parsing prob num:" << line << endl;
            cout << "Press any key to exit" << endl;
            fgetc(stdin);
            goto DONE;
        }

REDO:
        if(verb)cout << "checking problem:" << probNum << endl;
        if( probNum>maxp )maxp = probNum;

        int data[50][14] = {};
        input.getProb( probNum , data );

        string str[25];
        if(verb)cout << "getting 25 lines" << endl;
        for( int i = 0 ; i < 25 ; ++i )
        {
            getline(fs,str[i]);

            if( str[i] == "" )
            {
                cout << "got empty line , done" << endl;	
                goto DONE;
            }

            int prevNum = probNum;
            if( ERR != (probNum = conv(str[i])) )
            {
                cout << "find another $" << endl;
                emptyp.push_back(prevNum);
                goto REDO;
            }
			else
				probNum = prevNum;
        }

        bool error = false;
        for( int i = 0 ; i < 49 ; i+=2 )
        {
            int n = 0 , c = 0;
            if(verb)cout << "check line" << i << endl;
            for( int j = 0 ; j < 25 ; ++j ) 
            {
                if( str[j][i] == '1' ) 
                    n++;
                else if( str[j][i] == '0' || j == 24 )
                {
                    if( n != 0 ) 
                    {
                        if( data[i/2][c+1] != n ) 
                        {
                            printf("detect error\ncorrect answer %d\t(data[%d][%d])\n     ur answer %d\n",data[i][c+1],i,c+1,n);
                            error = true;
                        }
                        else
                        {
                            n = 0;    
                            c++; 
                        }
                    }
                }
                else
                {
                    cout << "WTF" << str[j][i] << endl; 
                }
            }
        }

        for( int i = 0 ; i < 25 ; ++i )
        {
            if(verb)cout << i << "." << endl;
            int n = 0 , c = 0;
            for( int j = 0 ; j < 49 ; j+=2 ) 
            {
                if( str[i][j] == '1' ) 
                    n++;
                else if( str[i][j] == '0' || j == 48 )
                {
                    if( n != 0 ) 
                    {
                        if( data[i+25][c+1] != n ) 
                        {
                            printf("detect error\ncorrect answer is %d\t(data[%d][%d])\n     ur answer %d\n",data[i+25][c+1],i+25,c+1,n);
                            error = true;
                        }
                        else
                        {
                            n = 0;    
                            c++; 
                        }
                    }
                }
                else
                {
                    cout << "WTF" << str[i][j] << endl; 
                }
            }
        }

        if( error != true )
        {
            if(verb)cout << "\tok" << endl;
            count++;
			
			// .second==false if element already existed
			if( solved.insert(probNum).second==false )
				printf("problem %d already solved before\n" , probNum );
        }
        else
        {
            if(verb)cout << "\twrong" << endl; 
            wrongp.push_back(probNum);
        }
    }
DONE:
    for( int k = 0 ; k < wrongp.size() ; ++k )
        cout << "wrong:" << wrongp[k] << endl;
    for( int k = 0 ; k < emptyp.size() ; ++k )
        cout << "emptyp:" << emptyp[k] << endl;
    cout << "total wrongs:" << wrongp.size() << endl;
    cout << "total emptys:" << emptyp.size() << endl;
    cout << "total solved:" << count << endl;
    cout << "max problem:" << maxp << endl;
    return 0;
}
