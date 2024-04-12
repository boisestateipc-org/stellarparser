//Goal is to create a text parser to more quickly spit out notes and relevant data fields from stellar
//Run this code with 2 arguments.  The first is the input text file (assumes path is in same folder), the second is name of output txt file

//EX: ./alertparser.exe inputFile.txt outputFile.txt
//returns 0 on success and 1 on failure

//************
//Next step is to check for other flags for the end of the alert - should be smallish
//Create nice ticket escalation file-  more simple, but a larger amount of code

//Interface a GUI - No idea, never done it.  Ranges from stupid easy to learning something entirely new

#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

int main(int argc, char* argv[]){
    
        //configure file variables
    string configBuff;
    string config = "configureParser.txt";
    ifstream configureFile(config);
    vector <string> checkList;

       //config file options
    bool start = false;
    bool silent = false;
    bool report = false;

    string reportFileName;

    
       //i and o stream
    string buffer;
    ifstream inFile(argv[1]); // first argument is inFile
    ofstream outFile(argv[2]); // second argument is outFile
    string tenantName;



        //config file error handling
        if(configureFile.is_open()){
            cout << "\nSuccessfully opened configuration file: " << config;
        }
        else{
            cout << "\nFailure to open configuration file: " << config << endl;
            return 1;
            };
     

        //Error checking for infile
        if (inFile.is_open()) {
            cout << "\nSuccessfully opened input file: " << argv[1] << endl;
        }
        else{
            cout << "\nFailure to open input file: " << argv[1] << endl;
            return 1;
            };


        //read in contents of config file
        //Added functionality for configure file settings
        while(configureFile >> configBuff){
            if(configBuff == "***REPORT***"){
                report = true;
                configureFile >> reportFileName;
                cout << "Generating a report with this file name: " << reportFileName << "\n";
                ofstream reportOutfile(reportFileName);
            }
            if(configBuff == "***SILENT***"){
                silent = true;
                cout << "Config file is on silent, shhhhhhh...\n";
            };
            if(configBuff == "***START***"){
              start = true;
              configureFile >> configBuff;  
              }
            if(start == true){
                checkList.push_back(configBuff);
                if(silent == false){cout << "Read in: " << configBuff << " from " << config << endl;}
              }
        }

        cout << endl; // Just for my sanity

        while(inFile >> buffer){
            if(buffer == "\"amsg\"" /*|| buffer =="\"\""*/){outFile << "\n\n";} //In most cases this will seperate alerts, i know there's at least one other option but can't find it


            for(int i = 0; i < checkList.size();i++){
               
                if(buffer == checkList[i]){

                      if(buffer == "\"description\":"){ //should grab whole line after description
                            outFile << buffer;
                            getline(inFile, buffer);
                            //cout << "buffer after getline: " << buffer << "\n";
                            outFile << buffer;
                            inFile >> buffer;
                        }
                        else{
                            outFile << buffer; //eg "dstip:"
                        }
                        
                        if(checkList[i] == "\"tenant_name\":"){ //grabs tenant name, haven't used it yet though
                            tenantName = checkList[i];
                        }
                        inFile >> buffer;
                        //perform some fun stuff on buffer to make it readable
                        
                        /*   
                        
                        
                        
                        */
                        //This convoluted piece of logic essentially comma delimits one relavant line...could have used getline?
                        if(buffer.back() != ',' && checkList[i] != "\"description\":"){ //if no , grab tokens until comma found, then use else for rest of things with commas
                         while(buffer.back() != ','){
                            outFile << buffer + " ";//initial push
                           // cout << "Found one withouth the \" in it: " << buffer << "\n";
                            inFile >> buffer;
                            //cout << "Pushing " << buffer << " onto the string.\n";
                            if(buffer.back()==','){outFile << buffer;}; //handles last token with the comma in it//fixes tenant name, but not description
                                                    }
                                                } 
                        
                        else{
                            outFile << buffer;
                            }
                            outFile << "\n"; 
                            break;
                                        }; //End of big if that checks buffer against checklist[i]
                                                     };//End of for loop that iterates through checklist

                                                 
                               };
 


    inFile.close();
    outFile.close();
    configureFile.close();

    return 0;
}