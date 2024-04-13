//Goal is to create a text parser to more quickly spit out notes and relevant data fields from stellar

//Run this code with 2 arguments.  The first is the input text file (assumes path is in same folder), the second is name of output txt file
  //Output file generated every time you run this, so it will overwrite files  of the same name
  //Input file must already Exist
  //Requires the "configureParser.txt" file to be in the same folder, and to have that exact name

//EX: ./alertparser.exe inputFile.txt outputFile.txt
//returns 0 on success and 1 on failure

//************


#include <iostream>
#include <fstream>
#include <vector>


using namespace std;

int main(int argc, char* argv[]){
        //general variables
        int currentMatchIndex{};
        int lastMatchIndex{};


        //configure file variables
    string configBuff;
    string config = "configureParser.txt";
    ifstream configureFile(config);
    vector <string> checkList;

       //config file options
    bool start = false;
    bool silent = false;
    bool report = false;
    
       //i and o stream
    string buffer;
    ifstream inFile(argv[1]); // second arg is inFile
    ofstream outFile(argv[2]); // third arg is outFile
    ofstream reportOutfile;
   

       //Report related variables
    string reportFileName;

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
            if(buffer == "\"amsg\"" || buffer =="\"amsg\","){outFile << "\n\n";} //In most cases this will seperate alerts, i know there's at least one other option but can't find it
            //I don't like using this value as a flag to seperate alerts, it seems consistent but could be volatile in the future

            for(int i = 0; i < checkList.size();i++){
               
                if(buffer == checkList[i]){ //  1) got a match from checklist
                    //this is where the index checking for alert seperation will go


                    outFile << buffer; // push the matched token
                    inFile >> buffer; // get our next token


                        //This block of logic first getlines everything to the next bracket if the token is a bracket | a little convoluted because geline grabs up to but not uncluding delim
                          //Or if the token's last character isn't a comma then grab the entire line (better than comma delim)
                          //It doesn't match either of those cases, just append that token to outfile

                        if(buffer == "["){             // 1)
                            outFile << buffer;
                            getline(inFile, buffer, ']');
                            outFile << buffer + "]";
                            inFile >> buffer;
                            outFile << buffer; // this adds the comma in case I want to csv the file later
                            inFile >> buffer; 
                        }
                        else if(buffer.back() != ',' ){ // 2) 
                            //inFile >> buffer;
                            outFile << buffer;
                            getline(inFile, buffer);
                            outFile << buffer;
                                                }
                        else{                           // 3)
                            outFile << buffer; 
                        }

                        inFile >> buffer; //grab our next token, ready for next iteration
                        outFile << "\n"; // newline character for outfile formatting

                                        }; //End of big if that checks buffer against checklist[i]
                                                     };//End of for loop that iterates through checklist

                                                 
                               };
 
        inFile.close();
        outFile.close();
        configureFile.close();
    
        /*
             if(report == true){
            cout << "Generating a report with this file name: " << reportFileName << "\n";
            reportOutfile.open(reportFileName);
        }


        reportOutfile.close();
        */
   




    return 0;
}