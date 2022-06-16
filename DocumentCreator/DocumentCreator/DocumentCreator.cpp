#include "Document.h"
#include "DocumentException.h"
#include <fstream>
#include <Windows.h>

#undef GetMessage

int main()
{
    std::ifstream fin;
    std::string filename;
    while (true)
    {
        std::cout << "Enter filename of document: ";
        getline(std::cin, filename);
        fin.open(filename);
        if (!fin.is_open())
        {
            std::cout << "Error: file can't be open (maybe it does not exist or is already open)";
            continue;
        }
        system("cls");
        try
        {
            Document document(fin);
            fin.close();
            std::cout << document.Render() << "\n";
            std::cout << "Click enter to continue or any other key to exit...";
            if (std::cin.get() != 10)
            {
                break;
            }
        }
        catch (const DocumentException& e)
        {
            std::cout << (std::string("Error: ") + e.GetMessage() + "\n");
        }
        std::cout << "Click enter to continue or any other key to exit...";
        if (std::cin.get() != 10)
        {
            break;
        }
        system("cls");
    }
}
