
// #include <bits/stdc++.h>
#include <map>
#include <algorithm>
#include <iostream>
#include <set>
#include <bitset>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <climits>
#include <cmath>
#include <vector>
using namespace std;
typedef long long ll;
double hit1 = 0;
double hit2 = 0;
ll no_of_branches = 0;
ll taken = 0;
ll not_taken = 0;
vector<string> hexa_pc_vect;
vector<string> hexa_mc_vect;
vector<ll> deci_pc_vect;
vector<string> instruction;
vector<string> branch_outcome;
vector<pair<string, string>> BTB;
map<string, string> mp;
// Function to convert a hexadecimal string to binary string without 0x and 0b
string hexStringToBinary(const string &hexString)
{
    // Skip "0x" prefix if present
    string hexValue = hexString;
    if (hexValue.substr(0, 2) == "0x")
        hexValue = hexValue.substr(2);

    // Convert hexadecimal string to integer
    stringstream ss;
    ss << hex << hexValue;
    ll intValue;
    ss >> intValue;

    // Convert integer to binary string
    std::bitset<32> bits(intValue);
    return bits.to_string();
}
// Function to convert a binary string to its hexadecimal equivalent
string binaryToHexadecimal(const string &binaryString)
{
    // Convert binary string to integer
    bitset<32> bits(binaryString);
    ll intValue = bits.to_ulong();

    // Convert integer to hexadecimal string
    stringstream ss;
    ss << "0x" << setfill('0') << setw(8) << hex << intValue;
    return ss.str(); // Return the string representation
}

ll hexToDecimal(const string &hexString)
{
    stringstream ss;
    ss << hex << hexString; // Set stringstream to interpret hex values

    ll result;
    ss >> result; // Extract integer value

    return result;
}
string inttohex(ll value)
{
    stringstream ss;
    ss << "0x" << std::setfill('0') << std::setw(8) << std::hex << value; // cout<<ss.str()<<" ";
    return ss.str();                                                      // Return the string representation
}
// to get the instruction whether it is branch or jal jalr or other instruction
string decode(string bin)
{
    string opcode;
    string funct3;
    for (int i = 25; i < 32; i++)
    {
        opcode += bin[i];
    }
    if (opcode == "1101111")
    {
        no_of_branches++;
        return "JAL";
    }
    else if (opcode == "1100011" || opcode == "1100111")
    {
        for (int i = 17; i < 20; i++)
        {
            funct3 += bin[i];
        }
        if (opcode == "1100011")
        {
            if (funct3 == "000")
            {
                no_of_branches++;
                return "beq";
            }
            else if (funct3 == "001")
            {
                no_of_branches++;
                return "bne";
            }
            else if (funct3 == "100")
            {
                no_of_branches++;
                return "blt";
            }
            else if (funct3 == "101")
            {
                no_of_branches++;
                return "bge";
            }
            else if (funct3 == "110")
            {
                no_of_branches++;
                return "bltu";
            }
            else if (funct3 == "111")
            {
                no_of_branches++;
                return "bgeu";
            }
        }
        else if (opcode == "1100111" && funct3 == "000")
        {
            no_of_branches++;
            return "JALR";
        }
    }
    return "no_bnc_inst"; // not a branch instruction
}

// if instruction is of branch and also if consecutive PC difference is 4 then immediate field of branch is calculated and in main is checked with consecutively next PC counter
string immediate(string bin, string inst)
{
    string offset;
    offset += bin[0];
    offset += bin[24];
    for (int i = 1; i < 7; i++)
    {
        offset += bin[i];
    }
    for (int i = 20; i < 24; i++)
    {
        offset += bin[i];
    }
    return offset;
}

// BRANCH TARGET BUFFER DISPLAY
void display_BTB()
{
    ofstream fi("Branch Target Buffer.txt");
    fi << "The branch target buffer is : \n";
    fi << "CURRENT BRANCH ADDRESS         TARGET ADDRESS" << endl; // BRANCH TARGET BUFFER
    for (int i = 0; i < BTB.size(); i++)
    {
        fi << BTB[i].first << "                       " << BTB[i].second << endl;
    }
}

void always_taken()
{
    vector<string> branch_prediction;
    double hit = 0; // TO FIND ACCURACY
    double accuracy = 0;
    double n = no_of_branches;
    for (int i = 0; i < no_of_branches; i++)
    {
        branch_prediction.push_back("T"); // PREDICTED BRANCH IS ALWAYS TAKEN
    }
    for (int i = 0; i < no_of_branches; i++)
    {
        if (branch_prediction[i].compare(branch_outcome[i]) == 0)
        {
            hit++;
        }
    }
    accuracy = (hit * 100 / n);
    // cout << hit << endl;
    cout << "Overall accuracy of ALWAYS_TAKEN branch predictor is: " << accuracy << "%" << endl;
}

void always_not_taken()
{
    vector<string> branch_prediction;
    double hit = 0; // TO FIND ACCURACY
    double accuracy = 0;
    double n = no_of_branches;
    for (int i = 0; i < no_of_branches; i++)
    {
        branch_prediction.push_back("N"); // PREDICTION IS BRANCH ALWAYS NOT TAKEN
    }
    for (int i = 0; i < no_of_branches; i++)
    {
        if (branch_prediction[i].compare(branch_outcome[i]) == 0)
        {
            hit++;
        }
    }
    //  cout << hit << endl;
    accuracy = (hit * 100 / n);
    cout << "Overall accuracy of ALWAYS_NOT_TAKEN branch predictor is: " << accuracy << "%" << endl;
}

void one_bit_dynamic(string ss)
{
    vector<char> branch_prediction;
    double accuracy = 0;
    double n = ss.size();
    double miss = 0;
    //  cout << "First prediction is branch not taken :\n";
    // cout<<ss<<endl;
    branch_prediction.push_back('N');
    for (int i = 0; i < ss.size(); i++)
    {
        if (ss[i] == ' ')
            continue;
        if (branch_prediction[branch_prediction.size() - 1] == ss[i])
        {
            branch_prediction.push_back(branch_prediction[branch_prediction.size() - 1]);
            hit1++;
        }
        else
        {
            branch_prediction.push_back(ss[i]);
            miss++;
        }
    }
}
void two_bit_dynamic(string ss)
{
    vector<char> branch_prediction;
    double hit = 0; // TO FIND ACCURACY
    double accuracy = 0;
    double miss = 0;
    //  cout << "First prediction is branch weakly taken :" << endl;
    branch_prediction.push_back('T');
    branch_prediction.push_back('N');
    for (int i = 0; i < ss.size(); i++)
    {
        if (ss[i] == ' ')
            continue;
        char dlep;
        string hlep;
        hlep += branch_prediction[branch_prediction.size() - 2];
        hlep += branch_prediction[branch_prediction.size() - 1];
        if (hlep == "NN" || hlep == "NT")
            dlep = 'N';
        else
            dlep = 'T';
        if (dlep == ss[i])
        {
            if (branch_prediction[branch_prediction.size() - 1] != ss[i])
            {
                branch_prediction[branch_prediction.size() - 1] = ss[i];
            }
            branch_prediction.push_back(dlep);
            hit2++;
        }
        else
        {
            if (hlep == "NN" || hlep == "TT")
                branch_prediction.push_back(ss[i]);
            else
            {
                if (hlep == "TN")
                    branch_prediction.push_back('T');
                else
                    branch_prediction.push_back('N');
            }
            miss++;
        }
    }
}
int main()
{
    ifstream in("input.txt"); // READING INPUT FILE
    while (!in.eof())         // PARSING FILE
    {
        string text;
        string pc; // PC VALUE WITHOUT 0X
        string mc; // MACHINE CODE WITHOUT 0X

        getline(in, text);
        int op = 0;
        string s;
        while (text[op] != ' ')
        {
            s += text[op];
            op++;
        }
        if (s != "core")
            continue;
        while (text[op] == ' ')
        {
            op++;
        }
        if (text[op] != '0')
            continue;
        op++;
        if (text[op] != ':')
            continue;
        op++;
        while (text[op] == ' ')
        {
            op++;
        }
        string s1;
        s1 += text[op];
        s1 += text[op + 1];
        if (s1 != "0x")
            continue;
        for (int i = 0; i < text.size(); i++)
        {
            if (text[i] == 'x')
            { // IGNORING CORE 0:
                i++;
                while (text[i] != ' ')
                { // GETTING PC VALUE
                    pc = pc + text[i];
                    i++;
                }
                while (text[i] != 'x' && i < text.size()) // IGNORING (0X  FOR MACHINE CODE
                    i++;
                if (text[i] == 'x')
                {
                    i++;
                    while (text[i] != ')')
                    { // GETTING MACHINE CODE AND IGNORING ITS CLOSING BRACKET
                        mc = mc + text[i];
                        i++;
                    }
                }
                hexa_pc_vect.push_back(pc); // COMPLETE LIST OF PC VALUES WITHOUT 0X IN HEXADECIMAL
                hexa_mc_vect.push_back(mc); // COMPLETE LIST OF MACHINE CODES WITHOUT 0X IN HEXADECIMAL
                break;
            }
        }
    }
    // STORING DECIMAL FORMAT OF ALL PC VALUES TO GET DIFFERENCE EASILY FURTHER
    for (int i = 0; i < hexa_pc_vect.size(); i++)
    {
        deci_pc_vect.push_back(hexToDecimal(hexa_pc_vect[i]));
    }

    // DECODING THE BRANCH AND JAL JALR INSTRUCTIONS AND CHECKING IF BRANCH TAKEN OR NOT AND STORING IT IN BRANCH_OUTCOME VECTOR
    for (int i = 0; i < hexa_mc_vect.size(); i++)
    {
        instruction.push_back(decode(hexStringToBinary(hexa_mc_vect[i]))); // INSTRUCTION IS DECODED BY CONVERTING MACHINE CODE TO BINARY FIRST AND THEN PASSING TO FUNCTION DECODE
        int j = i;
        // cout<<instruction[i]<<endl;
        //  IF INSTRUCTION IS A BRANCH INSTRUCTION
        if (i != hexa_mc_vect.size() && (instruction[i] == "beq" || instruction[i] == "bne" || instruction[i] == "blt" || instruction[i] == "bge" || instruction[i] == "bltu" || instruction[i] == "bgeu"))
        { // cout<<i<<" ";
            string imm = immediate(hexStringToBinary(hexa_mc_vect[i]), instruction[j]);
            string buff1 = hexa_pc_vect[j];
            string buff2 = binaryToHexadecimal(imm);
            //  cout<<buff2<<endl;
            ll b11 = hexToDecimal(buff2);
            b11 = b11 * 2;
            // cout<<b11<<endl;
            ll a11 = hexToDecimal(buff1);
            buff2 = inttohex(a11 + b11);
            //   cout<<buff2<<endl;
            if (deci_pc_vect[j + 1] - deci_pc_vect[j] != 4)
            {                                                //   cout<<buff2<<endl;                                          // CONSECUTIVE PC DIFFERENCE IS NOT 4 THEN IT IS SURELY BRANCH TAKEN
                string buffer = "0x" + hexa_pc_vect[j];      // CURRENT BRANCH ADDRESS AND ALSO TARGET ADDRESS
                string buffer2 = "0x" + hexa_pc_vect[j + 1]; // TARGET ADDRESS

                BTB.push_back({buffer, buffer2});
                branch_outcome.push_back("T"); // OUTCOME IS THAT BRANCH IS TAKEN
                mp[buffer] += " T";
            }
            // cout<< "0x"+ hexa_pc_vect[j + 1]<<" "<<buff2<<endl;
            else if ((("0x" + hexa_pc_vect[j]).compare(buff2)) == 0)
            {                                           // cout<<buff2<<endl;                                         // IF NEXT PC ADDRESS AND IMMEDIATE FIELD ARE EQUAL THEN IT MEANS LABEL WAS AT NEXT LINE AND BRANCH IS TAKEN
                branch_outcome.push_back("T");          // OUTCOME IS THAT BRANCH HAS BEEN TAKEN
                string buffer = "0x" + hexa_pc_vect[j]; // CURRENT BRANCH ADDRESS AND ALSO TARGET ADRESS
                BTB.push_back({buffer, buffer});
                mp[buffer] += " T";
            }
            else
            {                                              // IF NEXT PC VALUE AND IMMEDIATE ARE NOT SAME THEN BRANCH IS NOT TAKEN
                branch_outcome.push_back("N");             // OUTCOME IS THAT BRANCH IS NOT TAKEN
                string buffer1 = "0x" + hexa_pc_vect[j];   // CURRENT BRANCH ADDRESS
                string buffer2 = binaryToHexadecimal(imm); // TARGET ADDRESS
                ll a11 = hexToDecimal(buffer1);
                ll b11 = hexToDecimal(buffer2);
                b11 = 2 * b11;
                buffer2 = inttohex(a11 + b11);
                BTB.push_back({buffer1, buffer2});
                mp[buffer1] += " N";
            }
        }
        //  cout<<i<<endl;
        else if (i != hexa_mc_vect.size() && (instruction[i] == "JAL"))
        {
            //  cout<<i<<endl;
            string buffer = "0x" + hexa_pc_vect[j];
            // CURRENT BRANCH ADDRESS AND ALSO TARGET ADDRESS
            string buffer2 = "0x" + hexa_pc_vect[j + 1];
            // TARGET ADDRESS4
            BTB.push_back({buffer, buffer2});
            branch_outcome.push_back("T");
            mp[buffer] += " T";
        }
        else if (i != hexa_mc_vect.size() && (instruction[i] == "JALR"))
        {
            string buffer = "0x" + hexa_pc_vect[j];      // CURRENT BRANCH ADDRESS AND ALSO TARGET ADDRESS
            string buffer2 = "0x" + hexa_pc_vect[j + 1]; // TARGET ADDRESS
            BTB.push_back({buffer, buffer2});
            branch_outcome.push_back("T");
            mp[buffer] += " T";
        }
    }
    always_taken();
    always_not_taken();
    set<string> stt;
    for (auto x : BTB)
    {
        auto it = mp.find(x.first);
        if (it != mp.end() && stt.find(x.first) == stt.end())
        {
            string sss = it->second;
            one_bit_dynamic(sss);
            two_bit_dynamic(sss);
        }
        stt.insert(x.first);
    }
    // cout<<hit1<<" "<<hit2<<endl;
    double nn = no_of_branches;
    double accuracy = (hit1 * 100 / nn);
    // cout<<hit<<endl;
    cout << "Overall accuracy of one branch predictor is: " << accuracy << "%" << endl;

    accuracy = (hit2 * 100 / nn);
    // cout<<hit<<endl;
    cout << "Overall accuracy of two bit branch predictor is: " << accuracy << "%" << endl;
    ofstream ff("Original_outcomes.txt");

    ff << "Original branch outcomes:" << endl;
    for (int i = 0; i < no_of_branches; i++)
    {
        ff << branch_outcome[i] << endl;
    }
    // cout << endl;
    display_BTB();
    set<string> st;
    ofstream f("Branch History Table.txt");
    for (auto x : BTB)
    {
        auto it = mp.find(x.first);
        if (it != mp.end() && st.find(x.first) == st.end())
        {
            f << it->first << "                    " << it->second << "\n";
        }
        st.insert(x.first);
    }
}
