#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>

using namespace std;

vector<string> splitString(const string& input, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0, end = 0;

    while ((end = input.find(delimiter, start)) != string::npos) {
        tokens.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
    }

    // Handle the last token after the last delimiter
    tokens.push_back(input.substr(start));

    return tokens;
}

vector<vector<string>> splitString(const vector<vector<string>>& input){
    vector<vector<string>> resultMatrix;

    for (const auto& row : input) {
        vector<string> newRow;
        for (const auto& element : row) {
            istringstream iss(element);
            string token;
            while (getline(iss, token, ',')) {
                    newRow.push_back(token);
                }
        }
        resultMatrix.push_back(newRow);
    }

    return resultMatrix;
}

int main(int argc, char* argv[]) {
    ifstream myfile(argv[2]);
    vector<vector<string>> linesVector;
    string line;
    
    while(getline(myfile,line)){
        vector<string> indiv;
        indiv.push_back(line);
        linesVector.push_back(indiv);
    }
    myfile.close();
    //Good to here
    ifstream myfile2(argv[1]);
    vector<vector<string>> instructions;

    while(getline(myfile2,line)){
        line.erase(remove_if(line.begin(), line.end(), ::isspace), line.end());
        if(!line.empty() && line[0] == '/'){
            continue;
        }
        string delimiter = ",";
        vector<string> instr = splitString(line,delimiter);
        
        for (const string& element : instr) {
        }
        instructions.push_back(instr);
    }


    vector<string> states = instructions[1];
    vector<string> inputAlpha = instructions[2];
    vector<string> tapeAlpha = instructions[3];
    vector<string> initState = instructions[4];
    vector<string> blankSym = instructions[5];
    vector<string> finalState = instructions[6];
    
    string mode = linesVector[0][0];
    vector<vector<string>> ruleList(instructions.begin() + 7,instructions.end());

    vector<vector<string>> editedResult = splitString(ruleList);

    for (const auto& row : editedResult) {
        for (const auto& element : row) {
            cout << "[" << element << "]";
        }
        cout << endl;
    }
    
    string currentState = initState[0];
    int headPosition2 = 0;
    string headRead = "";
    string s1 = "(";

    if(mode == "Transducer"){
        string s1 = "(";
        for(int i=1;i<linesVector.size();i++){
            vector<string> b = linesVector[i];
            vector<string> a;
            for(const string& str :  b){
                for(const char& ch : str){
                    a.push_back(string(1,ch));
                }
            }
            string currentState = initState[0];
            int headPosition2 = 0;
            int count = 0;
            while(currentState != finalState[0]){
                if(headPosition2 < 0){
                    a.insert(a.begin(),"_");
                    headPosition2 = 0;
                }
                else if(headPosition2 >= a.size()){
                    a.insert(a.end(),"_");
                }
                for(int i = 0; i < editedResult.size();i++){
                    if(s1 + currentState == editedResult[i][0]){
                        if(a[headPosition2] == editedResult[i][1]){
                            currentState = editedResult[i][2];
                            a[headPosition2] = editedResult[i][3];
                            if(editedResult[i][4] == "R)"){
                                headPosition2++;
                                break;
                            }
                            else if(editedResult[i][4] == "L)"){
                                headPosition2--;
                                break;
                            }
                        }
                        else if(count <= 1000){
                            count ++; 
                        }
                        else{
                            currentState = finalState[0];
                            break;
                        }
                    }
                }
                if(currentState == finalState[0] && a[headPosition2] != "_"){
                    for (const auto& line:a){
                        for(const auto& element:line){
                            if(element == '1' || element == '0'){
                                cout << element << " " ;
                            }
                        }
                    } cout << endl;
                }
            }
        }
    }
    else if(mode == "Recognizer"){
        string s1 = "(";
        for(int i=1;i<linesVector.size();i++){
            vector<string> b = linesVector[i];
            vector<string> a;
            for(const string& str :  b){
                for(const char& ch : str){
                    a.push_back(string(1,ch));
                }
            }
            for(const auto& line: a){
                for(const auto& element:line){
                    cout << element << " ";
                }
                cout << endl;
            }
            string currentState = initState[0];
            int headPosition = 0;
            int count = 0;
            while(currentState != finalState[0]){
                if(headPosition >= a.size()){
                    a.insert(a.end(),"_");
                }
                else if(headPosition < 0){
                    a.insert(a.begin(),"_");
                }
                else if(a.size() == 2){
                    currentState = finalState[0];
                }
                for(int i = 0; i < editedResult.size();i++){
                    if(s1 + currentState == editedResult[i][0]){
                        if(a[headPosition] == editedResult[i][1]){
                            currentState = editedResult[i][2];
                            a[headPosition] = editedResult[i][3];
                            if(editedResult[i][4] == "R)"){
                                headPosition++;
                                break;
                            }
                            else if(editedResult[i][4] == "L)"){
                                headPosition --;
                                break;
                            }
                        }
                        else if(count <= 10){
                            count ++; 
                        }
                        else{
                            currentState = finalState[0];
                            break;
                        }
                    }
                }
                if(currentState == finalState[0] && a[headPosition] == "_"){
                    cout << "String Accepted" << endl;
                }
                else if (currentState == finalState[0] && a[headPosition] != "_"){
                    cout << "Rejected" << endl;
                }
            }
        }
    }return 0;
}