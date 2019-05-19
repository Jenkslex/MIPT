#include "SyntaxTree.hpp"
#include "Settings.hpp"
#include "Unicode.hpp"
#include "Output.hpp"
#include "Exception.hpp"


int main()
{
    try
    {
		printf("gtr2png %s by Nikitenko Evgeny and Ivanov Alexey.\n", GTR2PNG_VERSION);
        CSyntaxTree Tree(InputTreeFile);
        OutputTree("dot\\tree.dot", Tree);
		char Cmd[256] = "";
		ToUTF8("dot\\tree.dot");
		sprintf(Cmd, "dot\\dot.exe dot\\utf8_tree.dot -Tpng -o %s", OutputPngFile);
        system(Cmd);
        system(OutputPngFile);
    }
	catch (CException aError)
	{
	    aError.Print();
	    system("pause");
        return 1;
	}
	catch (CFileException aError)
	{
	    aError.Print();
	    system("pause");
        return 1;
	}
    catch (...)
    {
        printf("Unknown error occurred.\n");
        system("pause");
        return 1;
    }
    return 0;
}
