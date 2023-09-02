#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

std::string hexToString8(uint32_t value) {
    std::stringstream stream;
    stream << std::uppercase << std::hex << value;
    std::string result(stream.str());
    while (result.length() != 8) result = "0" + result;
    return result;
}

void printHex(std::string file) {
    std::ifstream myfile;
    myfile.open(file, std::ios::in | std::ios::binary);
    
    myfile.seekg (0, myfile.end);
    int length = myfile.tellg();
    myfile.seekg (0, myfile.beg);

    unsigned char* buffer = new unsigned char[length];
    myfile.read((char*) buffer, length);
    myfile.close();

    for (int i = 0; i < length; i += 4) {
        printf(".uint 0x%02X%02X%02X%02X\n", buffer[i+0], buffer[i+1], buffer[i+2], buffer[i+3]);
    }
}

int main(int argc, char* argv[]) {
    //system("cls");
    system("powerpc-eabi-g++ -Os -mcpu=750 -fshort-wchar -fno-inline -funroll-loops -fdata-sections -ffunction-sections -Wl,--gc-sections -Iinclude -Wl,-T,link.ld -o build/Main.o -std=gnu++17 Main.cpp");
    //system("powerpc-eabi-strip build/Main.o");
    system("powerpc-eabi-objcopy --only-section=.text build/Main.o -O binary build/text.bin");
    std::cout << "[Wrapper]" << std::endl;
    std::cout << "moduleMatches = 0x867317DE,0x6237F45C" << std::endl;
    std::cout << "" << std::endl;
    printf("0x02F37154 = b _onStart\n");
    std::cout << "" << std::endl;
    std::cout << "0x104D4DD8 = .uint _Code" << std::endl;
    std::cout << "0x104D4DDC = .uint 0x0" << std::endl;
    std::cout << "" << std::endl;
    std::cout << ".origin = codecave" << std::endl;
     std::cout << "" << std::endl;
    std::cout << "# .text" << std::endl;
    std::cout << "_Code:" << std::endl;
    printHex("build/text.bin");
    std::cout << "" << std::endl;
    std::cout << "_onStart:" << std::endl;
    std::cout << "bctrl" << std::endl;
    std::cout << "lis r12, _Code@ha" << std::endl;
    std::cout << "addi r12, r12, _Code@l" << std::endl;
    std::cout << "mtctr r12" << std::endl;
    std::cout << "bctrl" << std::endl;
    std::cout << "b 0x02F37158" << std::endl;
    return 0;
}