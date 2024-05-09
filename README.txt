There are many notes scattered throughout this project, this is intended to be the user direction file.
  -THERE ARE DIRECTIONS IN THE CONFIG FILE, READ THOSE AS WELL

1. Purpose - Quickly filter relevant fields from alerts and generate a preformatted Jira Ticket for potential escalation.

2. Requirements - There need to be 3 files in the same folder
    -The program itself
    -Input file - this is where you paste your  copied json text from the alerts
    -configureParser.txt - DON'T CHANGE THE NAME - this is where you will tell the parser what values you want
    -The output file is create/overwritten when the program is run and doesn't need to be created beforehand
    -The report outfile will be created automatically and overwrite any pre-existing file with the same name
    -Review the directions in the configureParser.txt file

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
     -EX "./.cyberParser.exe inputRawCaseData.txt parserOut.txt"
       -Different command line interfaces handle executing the program slightly differerently
       -Navigate to your new outfile and use a condensed version of what you were looking at 
       -You'll note a few bugs, I'm working on them.  But feel free to bring them to me and in case you find something I didn't know about

4. Known Issues
    -It will not run correctly unless the ***REPORT*** flag is set.
        -Not intended, however, it adds a whopping 10 ms to the run time so it's really low on my priorities to fix at the moment.
    -Occasionally there will be alerts that have multiple fields with the same name. 
        -Rare instance, but it doesn't seem to break the program, just doesn't give you the full picture.
    -The only consistent value I could find to flag an end to an alert was 'asmg'
        -It is unclear if this will be present in all alerts, so if for some reason your outfile didn't seperate alerts correctly, look there
    -The report generator logic can only handle values that are single word (token) or a sentence (getline)
        -Different from the alert parser, which should be able to handle structures in brackets and braces.
        -Since the purpose of this logic is to generate values for the Alerts section of the report, I think this is an acceptable limit to functionality.

Jordan Haines - Updated 2May2024
