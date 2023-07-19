#include "dpch.h"
#include "Application/DwarfEngine.h"

int main() {
	auto engine = Dwarf::Create();
    engine->Run();
    delete(engine);

    return 0;
}