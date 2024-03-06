#include "dpch.h"
#include "Application/DwarfEngine.h"

int main()
{
    auto engine = Dwarf::Create();
    engine->Run();

    return 0;
}