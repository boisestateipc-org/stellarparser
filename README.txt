There are many notes scattered throughout this project, this is intended to be the user direction file.

1. Purpose - Quickly filter and create a file with alert data that you deem relevant.

2. Requirements - There need to be 3 files in the same folder
    -The program itself
    -Input file - this is where you paste your  copied json text from the alerts
    -configureParser.txt - DON'T CHANGE THE NAME - this is where you will tell the parser what values you want
    -The output file is create/overwritten when the program is run and doesn't need to be created beforehand

3. Operation 
    -Navigate to your chosen Stellar Case in the Stellar Cyber Portal
    -Click on the i next to an alert
    -Click on JSON 
    -Click the copy to clipboard button near the top
    -Paste that data to whatever you've named your input file
      -Repeat for as many alerts as you have.
      -Ensure that the alert JSON text is copied to the end of previous alerts
      -SAVE the file
    -Run the program from your command line
     -parserprogram.exe yourInfile youroutputfile
     -EX "./.alertParser.exe inputRawCaseData.txt parserOut1.txt"
       -Different command line interfaces handle executing the program slightly differerently
       -Navigate to your new outfile and use a condensed version of what you were looking at 
       -You'll note a few bugs, I'm working on them.  But feel free to bring them to me and in case you find something I didn't know about


Jordan Haines - updated 12APR2024