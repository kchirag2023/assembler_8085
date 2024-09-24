#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

map<string, int> symbolTable;
map<string, int> instruction;
vector<string> program;
int codeAddress = 2000; 

void initializeInstructions() {
    instruction["MVI A"] = 0x3E;
    instruction["MVI B"] = 0x06;
    instruction["MVI C"] = 0x0E;
    instruction["MVI D"] = 0x16;
    instruction["MVI E"] = 0x1E;
    instruction["MVI H"] = 0x26;
    instruction["MVI L"] = 0x2E;
    instruction["MVI M"] = 0x36;

    // ADD Instructions
    instruction["ADD B"] = 0x80;
    instruction["ADD C"] = 0x81;
    instruction["ADD D"] = 0x82;
    instruction["ADD E"] = 0x83;
    instruction["ADD H"] = 0x84;
    instruction["ADD L"] = 0x85;
    instruction["ADD M"] = 0x86;
    instruction["ADD A"] = 0x87;

    // STA (Store Accumulator) Instruction
    instruction["STA"] = 0x32;

    // JMP (Unconditional Jump) Instruction
    instruction["JMP"] = 0xC3;

    // DCR (Decrement Register) Instruction
    instruction["DCR A"] = 0x3D;
    instruction["DCR B"] = 0x05;
    instruction["DCR C"] = 0x0D;
    instruction["DCR D"] = 0x15;
    instruction["DCR E"] = 0x1D;
    instruction["DCR H"] = 0x25;
    instruction["DCR L"] = 0x2D;
    instruction["DCR M"] = 0x35;

    // JNZ (Jump if Not Zero) Instruction
    instruction["JNZ"] = 0xC2;

    // HLT (Halt) Instruction
    instruction["HLT"] = 0x76;

    // MOV (Move Data) Instructions
    instruction["MOV A, B"] = 0x78;
    instruction["MOV A, C"] = 0x79;
    instruction["MOV A, D"] = 0x7A;
    instruction["MOV A, E"] = 0x7B;
    instruction["MOV A, H"] = 0x7C;
    instruction["MOV A, L"] = 0x7D;
    instruction["MOV A, M"] = 0x7E;
    instruction["MOV A, A"] = 0x7F;
    instruction["MOV B, A"] = 0x47;
    instruction["MOV B, B"] = 0x40;
    instruction["MOV B, C"] = 0x41;
    instruction["MOV B, D"] = 0x42;
    instruction["MOV B, E"] = 0x43;
    instruction["MOV B, H"] = 0x44;
    instruction["MOV B, L"] = 0x45;
    instruction["MOV B, M"] = 0x46;

    // SUB (Subtract) Instructions
    instruction["SUB B"] = 0x90;
    instruction["SUB C"] = 0x91;
    instruction["SUB D"] = 0x92;
    instruction["SUB E"] = 0x93;
    instruction["SUB H"] = 0x94;
    instruction["SUB L"] = 0x95;
    instruction["SUB M"] = 0x96;
    instruction["SUB A"] = 0x97;

    // INR (Increment Register) Instructions
    instruction["INR A"] = 0x3C;
    instruction["INR B"] = 0x04;
    instruction["INR C"] = 0x0C;
    instruction["INR D"] = 0x14;
    instruction["INR E"] = 0x1C;
    instruction["INR H"] = 0x24;
    instruction["INR L"] = 0x2C;
    instruction["INR M"] = 0x34;

    // CMP (Compare with Accumulator) Instructions
    instruction["CMP A"] = 0xBF;
    instruction["CMP B"] = 0xB8;
    instruction["CMP C"] = 0xB9;
    instruction["CMP D"] = 0xBA;
    instruction["CMP E"] = 0xBB;
    instruction["CMP H"] = 0xBC;
    instruction["CMP L"] = 0xBD;
    instruction["CMP M"] = 0xBE;

    // ANA (Logical AND with Accumulator) Instructions
    instruction["ANA B"] = 0xA0;
    instruction["ANA C"] = 0xA1;
    instruction["ANA D"] = 0xA2;
    instruction["ANA E"] = 0xA3;
    instruction["ANA H"] = 0xA4;
    instruction["ANA L"] = 0xA5;
    instruction["ANA M"] = 0xA6;
    instruction["ANA A"] = 0xA7;

    // ORA (Logical OR with Accumulator) Instructions
    instruction["ORA B"] = 0xB0;
    instruction["ORA C"] = 0xB1;
    instruction["ORA D"] = 0xB2;
    instruction["ORA E"] = 0xB3;
    instruction["ORA H"] = 0xB4;
    instruction["ORA L"] = 0xB5;
    instruction["ORA M"] = 0xB6;
    instruction["ORA A"] = 0xB7;

    // Other common instructions
    instruction["LDA"] = 0x3A;
    instruction["NOP"] = 0x00;
    instruction["CPI"] = 0xFE;

    // Add more instructions as needed
}

void firstPass(const string& code) {
    if (code.empty()) return;

    // Trim the line to ignore leading spaces
    string trimmedCode = code;
    trimmedCode.erase(remove(trimmedCode.begin(), trimmedCode.end(), ' '), trimmedCode.end());

    // Label handling
    if (trimmedCode[0] == '#') {
        string label = trimmedCode.substr(1);
        symbolTable[label] = codeAddress;
        codeAddress++;

    } else {
        // Increment code address for each valid instruction
        codeAddress++;
    }
}

void secondPass() {
    for (const string& line : program) {
        if (line.empty()) continue;

        
        string trimmedLine = line;
      

        // Handle labels
        if (trimmedLine[0] == '#') {
            continue; // Skip labels in second pass
        }

        // Find the first comma or space
        size_t commaPos = line.find(',');
        size_t spacePos = line.find(' ');

        string instructionName;
        string operand="";

        // Determine how to split the instruction and operand
        if(instruction.find(trimmedLine)!=instruction.end()){
            instructionName=line;
        }
        else if (commaPos != string::npos) {
            instructionName = line.substr(0, commaPos);
            operand = line.substr(commaPos + 1);
        } 
        else if (spacePos != string::npos) {
            instructionName = line.substr(0, spacePos);
            operand = line.substr(spacePos + 1);
        } else {
            instructionName = line; 
        }

        // cout<<endl<<"i : "<<instructionName<<" , "<<operand<<endl;

        // Trim the instruction name and convert to uppercase
        // instructionName.erase(remove(instructionName.begin(), instructionName.end(), ' '), instructionName.end());
        transform(instructionName.begin(), instructionName.end(), instructionName.begin(), ::toupper);

        // Check for unknown instruction
        if (instruction.find(instructionName) == instruction.end()) {
            cout << "Unknown instruction: " << instructionName << endl;
            continue;
        }

        cout <<std::hex<< instruction[instructionName] << endl; // Print opcode

        // Trim operand spaces
        operand.erase(remove(operand.begin(), operand.end(), ' '), operand.end());

        // Handle label operand (remove #)
        if (operand[0] == '#') {
            operand = operand.substr(1);
        }

        // If operand is a label, retrieve its address
        if (symbolTable.find(operand) != symbolTable.end()) {
            int A=symbolTable[operand];
             int a=A/100;
             int b=A%100;
             cout<<b<<endl;
             cout<<a<<endl;
            // cout << symbolTable[operand] << endl;
        } else {
            if(operand.empty()){
                continue;
            }
            // Handle numeric or hexadecimal operands
            try {
                if (operand.back() == 'H') {
                    string hexValue = operand.substr(0, operand.size() - 1);
                    int A=stoi(hexValue);
                    int a=A/100;
                    int b=A%100;
                    // cout << stoi(hexValue, nullptr, 16) << endl; // Convert hex to int
                    cout<<b<<endl;
                    cout<<a<<endl;
                } else {
                    cout << stoi(operand) << endl; // Convert decimal to int
                }
            } catch (const invalid_argument&) {
                cout << "Invalid operand: " << operand << endl;
            } catch (const out_of_range&) {
                cout << "Operand out of range: " << operand << endl;
            }
        }
    }
}

int main() {
    initializeInstructions();

    ifstream file("input.txt");
    if (!file) {
        cerr << "Unable to open file" << endl;
        return 1;
    }

    string line;
    while (getline(file, line)) {
        program.push_back(line); // Store the original line for the second pass
        firstPass(line); // Process the first pass
    }

    file.close();

    secondPass(); // Perform the second pass

    return 0;
}