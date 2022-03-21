#include"UID.h"

static std::random_device randomDevice;
static std::mt19937_64 engine(randomDevice());
static std::uniform_int_distribution<uint64_t> uniformDistribution;


UID::UID() : uuid(uniformDistribution(engine)){ }

UID::UID(uint64_t uuid){}