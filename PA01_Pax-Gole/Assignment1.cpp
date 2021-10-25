/*
    Name:       Pax Gole
    Class:      CS 3377.0W6
    Professor:  Dr. Nhut Nguyen
    Grader:     Ashutosh Nawale
    Due:        22 September 2021
    Program Desciption:
        This program takes in a URL and evaluates the protocol, domain, optional port,
        filepath, optional parameters, and general formatting for validity, then displays
        either the parsed information if valid or the errors found if invalid.
*/

#include <iostream>
#include <string>
using namespace std;

int validProctocol(const string & input, string &output);
int validDomain(const string & input, string &output, bool & checkPort);
int validPort(const string & input, string &output);
int validFilepath(const string & input, string &output);
int validParameters(const string & input, string &output);

//string urlOptions[] =
//        {"https://cs.utdallas.edu:80/directory/faculty/people.html?name=Robert", // good - https, port, params
//         "http://cs.utdallas.edu/directory/faculty/people.html", // good - plain
//         "httpz://cs.utdallas.euu:80/directory/faculty/people.html?name=Robert", // bad: Invalid Protocol, Invalid host
//         "https:/cs.utdallas.edu:80/directory/faculty/people.html?name=Robert" // bad: Unable to parse past protocol error
//        };

int main()
{
    string protOut,domainOut,portOut,fpOut,paramOut;
    int protVal, domainVal, portVal, fpVal, paramVal;
    string displayOutput;

    string inputURL;
    cout << "Enter a URL for parsing: ";
    cin >> inputURL;

    // check and display protocol
    protVal = validProctocol(inputURL,protOut);
    if (protVal == -1)
    {
        cout << protOut << endl;
        exit(0);
    }
    inputURL = string(inputURL,protVal,inputURL.size());
    cout << "Protocol: " <<  "      " << protOut << endl;


    // check domain
    bool checkPort;
    domainVal = validDomain(inputURL,domainOut,checkPort);
    if (domainVal == -1)
    {
        cout << domainOut << endl;
        exit(0);
    }
    inputURL = string(inputURL,domainVal,inputURL.size());
    cout << "Domain: " <<  "        " << domainOut << endl;


    // check port
    if (checkPort)
    {
        portVal = validPort(inputURL,portOut);
        if (portVal == -1)
        {
            cout << protOut << endl;
            exit(0);
        }
        inputURL = string(inputURL,portVal,inputURL.size());
        cout << "Port: " <<  "          " << portOut << endl;
    }

    // check filepath
    fpVal = validFilepath(inputURL,fpOut);
    if (fpVal == -1)
    {
        cout << fpOut << endl;
        exit(0);
    }
    inputURL = string(inputURL,fpVal,inputURL.size());
    cout << "Filepath: " <<  "      " << fpOut << endl;


    // check parameters
    if (inputURL.size() > 4)
    {
        if (inputURL.at(0) == '?')
        {
            paramVal = validParameters(inputURL,paramOut);
            if (paramVal == -1)
            {
                cout << paramOut << endl;
                exit(0);
            }
            cout << "Parameters: " <<  "    " << paramOut << endl;
        }
        else
            paramOut = "No parameters to check";
    }
    else
        paramOut = "No parameters to check";


    return 0;
}

int validProctocol(const string & input, string &output)
{
    // check for end of component (format errors)
    string protEnd = "://";
    int returnVal = static_cast<int>(input.find(protEnd)); // default error setting
    if (returnVal == -1) // if not, continue to next check
    {
        output = "Protocol format error, unable to parse further";
        returnVal = -1; // error 1 - format invalid
        return returnVal;
    }
    else // if yes, prepare index to return
        returnVal += 3;

    // check for valid component
    string acceptableProtocols[3] = {"https://","http://","ftp://"};
    bool found = false;
    string checkProt = string(input,0,returnVal);
    for (const auto& currProt : acceptableProtocols) // for each possible protocol
    {
        if (currProt == checkProt) // if found
        {
            found = true;
            output = string(currProt,0,currProt.size()-3); // save
            break;
        }
    }
    if (!found) // if not found
    {
        output = string(checkProt,0,checkProt.size()-3);
        output += " is an invalid protocol - it must be \"http\",\"https\", or \"ftp\""; // error 2
        // error 2 - protocol invalid, index correct (unmodified returnVal)
    }
    return returnVal;
}

int validDomain(const string & input, string &output, bool & checkPort)
{
    // check index of port beginning to verify formatting
    int returnVal = static_cast<int>(input.find(':'));
    if (returnVal == -1) // if not found, check filepath beginning
    {
        returnVal = static_cast<int>(input.find('/')); // check for lack of port
        if (returnVal == -1)
        {
            output = "Domain format error, unable to parse further";
            return returnVal; // error 1 - format invalid
        }
        else // set port to false
        {
            checkPort = false;
        }
    }
    else // else remember to check for port
    {
        checkPort = true;
    }

    // verify domain format
    string domainCheck = string(input,0,returnVal);

    // check formatting
    string tempDomain = domainCheck;
    int demarcCount[2]; // num of demarcations by periods
    demarcCount[0] = static_cast<int>(tempDomain.find('.')); // set two expected demarcations
    demarcCount[1] = static_cast<int>(tempDomain.rfind('.'));
    if (demarcCount[0] == demarcCount[1]) // in case of 1 or 0 periods in domain
    {
        output = "Domain format does not match: must be in form 'x.y.z'";
        return returnVal;
    }
    tempDomain.erase(demarcCount[0],demarcCount[0]-1);
    tempDomain.erase(demarcCount[1]-1);
    for (char i : tempDomain) // check alnum of string
    {
        if (!isalnum(i))
        {
            output = "Domain format 'x.y.z' is not alphanumeric";
            return returnVal;
        }
    }

    // verify host validity
    string host = string(domainCheck,domainCheck.size()-3);
    string acceptableHosts[] = {"com", "net", "edu", "biz", "gov"};
    bool found = false;
    for (const auto& currHost : acceptableHosts)
    {
        if (currHost == host)
        {
            found = true;
            output = domainCheck;
            break;
        }
    }
    if (!found)
    {
        output = host;
        output += "is an invalid host - it must be \"com\", \"net\", \"edu\", \"biz\", or \"gov\""; // error 2
        // error: wrong host
    }
    return returnVal;
}

int validPort(const string & input, string &output)
{
    int returnVal = static_cast<int>(input.find('/'));

    // check format
    if ((input.at(0) != ':') || (returnVal == -1))
    {
        returnVal = -1;
        output = "Port format error, unable to parse further";
        return returnVal;
    }
    string checkPort = string(input,1,returnVal-1);
    int len = static_cast<int>(checkPort.size());
    for (int i = 0; i < len; i++) // check alnum of string
    {
        if (!isdigit(checkPort.at(i)))
        {
            output = checkPort + " is an invalid port number, non digits parsed - check filepath format for missing parts";
            return i+1;
        }
    }
    // check if within range
    int checkPortInt = stoi(checkPort);
    if (!((checkPortInt <= 65535) && (checkPortInt >= 1)))
    {
        output = "Port number must be between 1 and 65535";
        return returnVal;
    }
    else
    {
        output = checkPort;
        return returnVal;
    }
}

int validFilepath(const string & input, string &output)
{
    if (input.at(0) != '/') // check formatting
    {
        output = "Filepath format error, unable to parse further";
        return -1; // error 1 - format invalid
    }
    int returnVal = static_cast<int>(input.find(".htm")) + 4; // check end of filepath
    if (returnVal == -1)
    {
        output = "Filepath format error, unable to parse further";
        return returnVal; // error 1 - format invalid
    }
    int checkParam = static_cast<int>(input.find('?'));
    if (checkParam != -1)
    {
        output = string(input,0,returnVal);
        if (input.at(returnVal) == 'l')
        {
            returnVal = returnVal + 1;
            output += 'l';
        }
    }
    else
    {
        output = string(input, 0);
    }
    return returnVal;
}

int validParameters(const string & input, string &output)
{
    if (input.at(0) != '?')
    {
        output = "Parameters must begin with \'?\'";
        return -1;
    }
    // format validated output
    output = string(input,1,input.size());
    string replacer = ", ";
    for (int i = 0; i < output.size(); i++) // check alnum of string
    {
        if (output.at(i) == '&')
        {
            output.replace(i,1,replacer);
        }
    }
    return 0;
}