//Goal is to create a text parser to more quickly spit out notes and relevant data fields from stellar and generate potential Jira report

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
#include <time.h>

using namespace std;

string boldString(string boldThis);
string sanitizeString(string sanitizeThis);
string italicString(string italicThis);
string convertUnix(string unixTime);


int main(int argc, char* argv[]){
        //general variables
        int currentMatchIndex{};
        int lastMatchIndex{};
        int frontBraceCount{};
        int backBraceCount{};


        //configure file variables
    string configBuff;
    string config = "configureParser.txt";
    ifstream configureFile(config);
    vector <string> checkList;

       //config file options
    bool start = false;
    bool silent = false;
    bool report = false;
    bool reportStart = false; //  should only need to turn on or off, 

    
       //i and o stream
    string buffer;
    string  tempBuff;
    ifstream inFile(argv[1]); // second arg is inFile
    ofstream outFile(argv[2]); // third arg is outFile
    ofstream reportOutfile;
    ifstream inFileReport;
    
       
       //The goal of this structure is to support "Less-Than-Optimal-User Proofing" the configFile and extensibility. 
       //These two structures are used in tandem and are arranged like this:
           //reportHeaderData contains values that will not change - values from configFile
           //reportValueData is for parsed data from the JSON and will be utilized as a vector with each object being an Alert

    struct reportHeaderData{
        int reportAlertCount = 0; // this will track how many objects of the reportValueData we have. 
        string reportFileName{};
        vector <string> reportAlertFields{};
        string tableFieldSeperator = "|  ";
    };

    struct reportValueData {
        vector <string> reportAlertValues{};
    };

    reportHeaderData reportHeader{};
    vector <reportValueData> reportDataVector{}; //This is potentially fragile, I'm banking on amsg to flag needing new objects
    string reportBuff{}; // Buffer for report later
    int k = 0; // For checking against the alert fields
    bool alertFound = false; 

    reportDataVector.push_back({}); // index 0 of vector of structures that contains vectors. That has to be computationally complex



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


        //Parse Logic for Config File
        //Flag Logic for Config File
        while(configureFile >> configBuff){
            if(configBuff == "***REPORT***"){  
                report = true;
                configureFile >> configBuff;
                reportHeader.reportFileName = configBuff;
                continue;
            }
            if(configBuff == "***SILENT***"){ 
                silent = true;
                continue;
            };
            if(configBuff == "***START***"){ 
              start = true;
              continue;
              }
            if(configBuff == "***REPORTEND***"){
              reportStart = false;
              continue;
            }
            if(configBuff == "***REPORTSTART***"){
              reportStart = true;
              continue;
            }
            
            //Action Logic
            if(reportStart == true){
                reportHeader.reportAlertFields.push_back(configBuff);
                reportHeader.tableFieldSeperator += "---------------  |  ";
                reportDataVector[0].reportAlertValues.push_back(" No Value Found ");//primes value vector into first alertValues struct and makes a seperation for the table at index 0

                if(silent == false){cout << "Read in: " << configBuff << " from " << config << " for report alerts." << endl;}
               
            }
            if(start == true){
                checkList.push_back(configBuff);
                if(silent == false){cout << "Read in: " << configBuff << " from " << config << endl;}
                
              }
            
        }

        cout << endl; // Just for my sanity

    //Just prints out Key Fields for Jira ticket if the silent flag is not present    
    if(silent == false){
        for(int i = 0; i < reportHeader.reportAlertFields.size(); i++){
           cout << "\nKey Fields for Jira report: " << reportHeader.reportAlertFields[i];
        }
    }
       int l = 0;

        while(inFile >> buffer){
            reportBuff = "";
            k = 0;
            alertFound = false;

            for(int i = 0; i < size(reportHeader.reportAlertFields);i++){
                if(buffer == reportHeader.reportAlertFields[i]){
                    k = i;
                    alertFound = true;
                    break;
                }
            }
        
             
            //this seperates alerts and creates a new structure for
            if(buffer == "\"amsg\"" || buffer =="\"amsg\","){
                outFile << "***ENDOFALERT***" << "\n\n"; // ***ENDOFALERT***

                 reportDataVector.push_back({}); //creates an instance of structure in vector
                  reportHeader.reportAlertCount++;// Counts number of alerts for use in 

                    //if there's a report we need default values    
                    if(report == true){
                         for(int i = 0 ; i < size(reportHeader.reportAlertFields); i++){
                            reportDataVector[reportHeader.reportAlertCount].reportAlertValues.push_back("No Value Found");  
                       }
                    }             
                 
                } //In most cases this will seperate alerts, i know there's at least one other option but can't find it
            //I don't like using this value as a flag to seperate alerts, but it seems consistent and I lack a better solution that
             //isn't unnecessarily intensive

            for(int i = 0; i < checkList.size();i++){
               
                if(buffer == checkList[i]){ //  1) got a match from checklist
                     if(buffer.find("time") != string::npos)
                     {
                             outFile << buffer+" ";
                             inFile >> buffer;
                             buffer = convertUnix(buffer); //Turns it to readable unix time
                             buffer = sanitizeString(buffer);
                             buffer +=",";
                             
                            }
                      else{
                           outFile << buffer+" "; // push the matched token field
                           inFile >> buffer; // get our next token
                        }
                    


                        //This block of logic first getlines everything to the next bracket if the token is a bracket |
                        //a little convoluted because geline grabs up to but not uncluding delim
                          //Or if the token's last character isn't a comma then grab the entire line (better than comma delim)
                          //It doesn't match either of those cases, just append that token to outfile

                        if(buffer == "["){             // 1)
                            outFile << buffer;
                            getline(inFile, buffer, ']');
                            outFile << buffer + "],";
                            outFile << "\n";
                            continue;                         
                        }
                        //appends getline to buffer until and even amount of forward and backward braces are found.
                        else if(buffer == "{"){     
                            frontBraceCount = 1;
                            backBraceCount = 0;

                            while(frontBraceCount != backBraceCount){
                                getline(inFile, tempBuff);
                                buffer += "\n"; 
                                buffer += tempBuff;
                                    if(tempBuff.find('{') != string::npos){ // { found
                                        frontBraceCount++; 
                                    }
                                    else if(tempBuff.find('}') != string::npos){ // } found
                                       backBraceCount++; 
                                    }
                           } 
                            outFile << buffer; 
                            outFile << "\n";
                            continue;
                        } //end brace logic

                        else if(buffer.back() != ',' ){ //if not a brace, or a bracket, or have a comma, then push and getline
                            //inFile >> buffer;
                            outFile << buffer;
                            reportBuff += buffer;
                            // cout << "buffer comma before getline: " << buffer << "\n";
                            getline(inFile, buffer);
                            reportBuff += buffer;
                            // cout << "buffer comma AFTER getline: " << buffer << "\n";
                            outFile << buffer;
                            outFile << "\n";
                            if(alertFound==true){
                                   reportDataVector[reportHeader.reportAlertCount].reportAlertValues[k] = reportBuff;
                                }
                            continue;

                                                }
                        else{                // then just push it
                                                                 
                            outFile << buffer; 
                            reportBuff = buffer;
                            outFile << "\n";
                            if(alertFound==true){
                                   reportDataVector[reportHeader.reportAlertCount].reportAlertValues[k] = reportBuff;
                               }
                            continue;
                        }

                        inFile >> buffer; //grab our next token, ready for next iteration
                        outFile << "\n"; // newline character for outfile formatting

                                        }; //End of big if that checks buffer against checklist[i]

                                                     };//End of for loop that iterates through checklist

          

                                                 
                               };
    
        inFile.close();  
        outFile.close();
        configureFile.close();
    
        
    /*So begins the actual Jira Report Generation 
      -inFile is now what was previously the outfile
    */
  
           

   // ifstream inFileReport(argv[2]); //LOOK AT ME, I AM DE INFILE NOW

    if(report == true){

    reportOutfile.open(reportHeader.reportFileName);

    if (reportOutfile.is_open()) {
        cout << "Generating a report with this file name: " << reportHeader.reportFileName << "\n\n";
        }
    else{
        cout << "Failure to open report file: " << reportHeader.reportFileName << endl;
        return 1;
        };



        reportOutfile << "# _Case:_ " << " _PLACETEXTHERE_\n\n";
          
        reportOutfile << "# SUMMARY\n";
        reportOutfile << "### PLACE SUMMARY HERE\n\n";

        reportOutfile << "# RECOMMENDATIONS\n";
        reportOutfile << "\t_PLACE BULLET POINTS HERE\n";

        reportOutfile << "# REQUESTED INFORMATION\n\n";
        reportOutfile << "\t_PLACE BULLET POINTS HERE\n";

        reportOutfile << "# ALERTS\n\n";
           

    //Alert Table Header generator for output report file
    
    reportOutfile << "|  ";
       //cout << "|  ";
       for(int i = 0; i < size(reportHeader.reportAlertFields) ; i++){

         //cout << "\n" <<  reportHeader.reportAlertFields[i];
         reportOutfile <<  reportHeader.reportAlertFields[i] << "  |";

         }
    reportOutfile << "\n";
       


       // reportOutfile << "| Alert Type     |     Time      | Destination IP | Source IP | Description |\n";
       reportOutfile << reportHeader.tableFieldSeperator;
       reportOutfile << "\n";

    //Fills fields after Alert Header
        for(int i = 0 ; i < reportHeader.reportAlertCount; i++){
               // cout << "\n" << "|   "; 
                reportOutfile << "|   "; 
             for(int j = 0; j < size(reportHeader.reportAlertFields) ; j++){
                 // cout << " " <<  reportDataVector[i].reportAlertValues[j] << "   |";
                  reportOutfile << " " <<  reportDataVector[i].reportAlertValues[j] << "   |";
             }
             reportOutfile << "\n";
        }
    reportOutfile.close();
    outFile.close();


    }//End of the report generation

    

    return 0;
}

///////////////////////////Function Junction//////////////////////////////////////


//removes end characters that aren't printable from strings by checking ASCII values
string sanitizeString(string sanitizeThis){
    int printableASCII =  33; //printable characters start at 33 in ASCII

    int indexCheck = sanitizeThis.length()-1;
    char ascii = sanitizeThis[indexCheck];
    int asciiNum = ascii; //convert character to an ascii number
   
    while(asciiNum < printableASCII){
        sanitizeThis.erase(sanitizeThis.length()-1);
        indexCheck = sanitizeThis.length()-1;
        ascii = sanitizeThis[indexCheck];
        asciiNum = ascii;
    }
    return sanitizeThis;
}

//Makes the string Markdown Bold
string boldString(string boldThis){
    string buffer = "**";
    buffer+= boldThis;
    buffer+="**\n";
    return buffer;
};

//makes string Markdown Italics
string italicString(string italicThis){
    string buffer = "*";
    buffer+= italicThis;
    buffer+="*\n";
    return buffer;
};

//Take unix ms time string with comma, cut comma, convert to readable
  //and return as string

string convertUnix(string convertThis){
    string timeOut;
    convertThis.erase(convertThis.length()-1);

    time_t datetime_ms = stoll(convertThis);
    datetime_ms = datetime_ms/1000;

    convertThis = ctime(&datetime_ms);

    return convertThis;
}