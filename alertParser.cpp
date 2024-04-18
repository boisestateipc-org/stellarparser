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

string boldString(string boldThis);
string sanitizeString(string sanitizeThis);
string italicString(string italicThis);


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

    string endOfAlertFlag = "***ENDOFALERT***";
    string silentFlag = "***SILENT***";
    string reportFlag = "***REPORT***";
    string startFlag = "***START***";
    string reportStartFlag = "***REPORTSTART***";
    string reportEndFlag = "***REPORTEND***";


    
       //i and o stream
    string buffer;
    string  tempBuff;
    ifstream inFile(argv[1]); // second arg is inFile
    ofstream outFile(argv[2]); // third arg is outFile
    ofstream reportOutfile;
   

       //Report related variables
    string reportFileName;
    vector <string> reportAlertFields; // string matrix for report stuff
    vector <string> reportAlertValues;


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

               //FLAG True False Logic 
            if(configBuff == reportFlag){  //***REPORT***
                report = true;
                configureFile >> reportFileName;
            }
            if(configBuff == silentFlag){ // ***SILENT***
                silent = true;
                cout << "Config file is on silent, shhhhhhh...\n";
            };
            if(configBuff == startFlag){ //***START***
              start = true;
              configureFile >> configBuff;  
              }
            if(configBuff == reportEndFlag){//***REPORTEND***
              reportStart = false;
              configureFile >> configBuff; 
            }
            if(configBuff == reportStartFlag){//***REPORTSTART***
              reportStart = true;
              configureFile >> configBuff; 
            }

            //Action Logic
            if(reportStart == true){
                reportAlertFields.push_back(configBuff);
                if(silent == false){cout << "Read in: " << configBuff << " from " << config << " for report alerts." << endl;}
            }
            if(start == true){
                checkList.push_back(configBuff);
                if(silent == false){cout << "Read in: " << configBuff << " from " << config << endl;}
              }
        }

        cout << endl; // Just for my sanity

        while(inFile >> buffer){
            if(buffer == "\"amsg\"" || buffer =="\"amsg\","){
                outFile << endOfAlertFlag << "\n\n"; // ***ENDOFALERT***
                
                } //In most cases this will seperate alerts, i know there's at least one other option but can't find it
            //I don't like using this value as a flag to seperate alerts,but it seems consistent
             
             /* if(lastMatchIndex > currentMatchIndex){
                outFile << "***ENDOFALERT***" << "\n\n"; // this could be an issue for parsing later
                lastMatchIndex = -1;
                //
            }  
            */   



            for(int i = 0; i < checkList.size();i++){
               
                if(buffer == checkList[i]){ //  1) got a match from checklist
                    //this is where the index checking for alert seperation will go
                  /*  lastMatchIndex = currentMatchIndex;
                    currentMatchIndex = i;*/
                    //This didn't work, I believe due to the difference in alerts it was possible to cycle completely through an alert
                        // and still end up with an index value lower than your first hit in the next alert. User could also cause errors.

                    outFile << buffer+" "; // push the matched token field
                    inFile >> buffer; // get our next token


                        //This block of logic first getlines everything to the next bracket if the token is a bracket | a little convoluted because geline grabs up to but not uncluding delim
                          //Or if the token's last character isn't a comma then grab the entire line (better than comma delim)
                          //It doesn't match either of those cases, just append that token to outfile

                        if(buffer == "["){             // 1)
                            outFile << buffer;
                            getline(inFile, buffer, ']');
                            outFile << buffer + "],";
                            outFile << "\n";
                            break;
                            //inFile >> buffer;
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
                            break;
                            //inFile >> buffer; 
                        } //end brace logic

                        else if(buffer.back() != ',' ){ //if not a brace, or a bracket, or have a comma, then push and getline
                            //inFile >> buffer;
                            outFile << buffer;
                            // cout << "buffer comma before getline: " << buffer << "\n";
                            getline(inFile, buffer);
                            // cout << "buffer comma AFTER getline: " << buffer << "\n";
                            outFile << buffer;
                            outFile << "\n";
                            break;

                                                }
                        else{                           // then just push it
                            outFile << buffer; 
                            outFile << "\n";
                            break;
                        }

                        inFile >> buffer; //grab our next token, ready for next iteration
                        outFile << "\n"; // newline character for outfile formatting

                                        }; //End of big if that checks buffer against checklist[i]

                                                     };//End of for loop that iterates through checklist

          

                                                 
                               };
 
        inFile.close(); // then reuse for 
        outFile.close();
        configureFile.close();
    
        
    /*So begins the actual Jira Report Generation 
      -inFile is now what was previously the outfile
      -every line of text will need to be scrubbed as it goes into it
      -structures?
      -Do I need to backtrack and create a bunch of code that grabs these items for storage later?
      -Or is it more effecient to parse my newly created document, I know what the format will be pretty absolutely
      */

    if(report == true){
        cout << "Generating a report with this file name: " << reportFileName << "\n\n";
        reportOutfile.open(reportFileName);
        ifstream inFile(argv[2]); //LOOK AT ME, I AM DE INFILE NOW

        string testLine = "This is a test string for testing purposes\n\n\n";
        //This is going to follow atlassian markdown convention

        reportOutfile << "# _Case_ " << " _PLACETEXTHERE_\n\n";
        reportOutfile << "# TESTLINE\n";

        testLine = sanitizeString(testLine);
        testLine = boldString(testLine);
        reportOutfile << testLine;
        
        reportOutfile << "# SUMMARY\n";
        reportOutfile << "## PLACE SUMMARY TEXT BOX HERE\n\n";

        reportOutfile << "# RECOMMENDATIONS\n";
        reportOutfile << "\t_PLACE BULLET POINTS HERE\n";

        reportOutfile << "# REQUESTED INFORMATION\n\n";
        reportOutfile << "\t_PLACE BULLET POINTS HERE\n";

        reportOutfile << "# ALERTS\n\n";
           
        //now to turn it into a loop that does this for you based on end tokens
        //a string matrix has the benefit of being more easily extensible to my mind
        //a structure is more elegant, but all my fields will be strings, except time.

        
        reportOutfile << "| Alert Type     |     Time      | Destination IP | Source IP | Description |\n";
        reportOutfile << "| ---------------|---------------|----------------|-----------|-------------|\n";
        reportOutfile << "| Monday    im all about words words, see what happens when this stuff is longer great great great but what happens if this nonsense is truly a ridiculous lenght.  I'm talkin mongo bongo ding dilli dong length.  Absolutely tim and tom foolishness in how stupid long this thing gets.    |      Time      | Destination IP | Source IP | Description |\n";
        reportOutfile << "| Tuesday |     Time      | Destination IP | Source IP | Description |\n";




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
        //cout << "sanitizeThis last character: " << sanitizeThis[indexCheck] << endl;
        //cout << "\nASCII value of last character: " << asciiNum << endl;
        sanitizeThis.erase(sanitizeThis.length()-1);
       // cout << "Sanitizing...\n";
        
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
}