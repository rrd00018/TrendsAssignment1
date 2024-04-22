/*
 * ./main A1 A2 A3 A4 att(1,2) att(2,1) att(1,4)
 */


#include <iostream>
#include "argumentSolver.h"
#include "argumentBuilder.h"
#include <Windows.h>

int main(int argc, char* argv[]) {
    OPENFILENAME ofn;
    TCHAR szFile[260] = { 0 };

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";

    std::cout << szFile << std::endl;
    std::string path1 = "C:\\Users\\Rafa\\Documents\\UJA\\Tercero\\IA\\input.txt";
    argumentBuilder builder;
    builder.loadData("C:\\Users\\Rafa\\Documents\\UJA\\Tercero\\IA\\input.txt");
    builder.run();
    //argumentSolver(argc,argv);

    return 0;
}