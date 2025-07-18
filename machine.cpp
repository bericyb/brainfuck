#include <cctype>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

// #define DEBUG

class Machine {

public: 
    Machine() {
    }
    std::string printMem(int start, int end) {
        std::string strres = "";
        std::string intres = "";

        for (int i = start; i < end; i++) {
            strres += (char)memory[i] + ' ';
            intres += std::to_string((int)memory[i]) + ' ';
        }

        return strres + '\n' + intres + '\n';
    }

    void execute(std::string line) {
        for (int i = 0; i < line.size(); i++) {
            #ifdef DEBUG
            std::cout << line[i];
            #endif
            switch (line[i]) {
                case '>':
                    incPtr();
                    break;
                case '<':
                    decPtr();
                    break;
                case '+':
                    incVal();
                    break;
                case '-':
                    decVal();
                    break;
                case '[':
                    {
                        int loopEnd = startLoop(line, i);
                        if (loopEnd != -1) {
                            i = loopEnd;
                        }
                        break;
                    }
                case ']':
                    {
                        int loop = endLoop();
                        if (loop != -1) {
                            i = loop;
                        }
                        break;
                    }
                case '.':
                    std::cout << getVal();
                    break;
                case ',':
                    inputVal();
                    break;
                default:
                    break;
            }
        }
    }

private: 
    unsigned char memory[30000] = { };
    int m_ptr = 0;
    std::stack<int> loops;

    void incPtr() {
        if (m_ptr < 30000) {
            m_ptr = m_ptr + 1;
        } else {
            exit(3);
        }
    }
    void decPtr() {
        if (m_ptr > 0) {
            m_ptr = m_ptr - 1;
        } else {
            exit(1);
        }
    }
    void incVal() {
        memory[m_ptr]++;
    }
    void decVal() {
        memory[m_ptr]--;
    }
    char getVal() {
        return memory[m_ptr];
    }
    void inputVal() {
        char val;
        std::cin >> val;
        memory[m_ptr] = val;
    }
    int startLoop(std::string line, int i) {
        // Skip the loop if zero
        if (memory[m_ptr] == 0) {
            std::stack<int> stack;
            for (int idx = i; idx < line.length(); idx++) {
                if (line[idx] == '[') {
                    stack.push(idx);
                }
                if (line[idx] == ']') {
                    int match = stack.top();
                    if (match == i) {
                        return idx;
                    }
                    stack.pop();
                }
            }
        } else {
            loops.push(i);
        }
        return -1;
    }

    int endLoop() {
        if (memory[m_ptr] == 0) {
            loops.pop();
            return -1;
        } else {
            return loops.top();
        }
    }
};




int main(int argc, char **argv) {
    Machine m = Machine();
    std::cout << "opening file: " << argv[1] << '\n';

    std::ifstream infile(argv[1]);

    std::string file;
    std::string line;

    while (std::getline(infile, line)){
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        file += line;
    }

    // std::cout << file << '\n';
    m.execute(file);

    std::cout << "\n\n" << m.printMem(0, 10) << '\n';
}
