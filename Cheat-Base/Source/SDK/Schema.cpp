#include "Schema.h"

#include <deque>

#include "../Interfaces.h"
#include "../Definitions.h"

#include "Source2/SchemaSystem.h"

struct SchemaOffset {
    hash32_t m_class;
    hash32_t m_prop;
    short m_offset;
};

static std::unordered_map<hash32_t, std::unordered_map<hash32_t, SchemaOffset>> schemaData;

std::vector<std::string> netvarModules = {
    CLIENTDLL,
    ENGINE2DLL,
};

void InitModule(const char* moduleName)
{
    auto findType = interfaces->schemaSystem->findTypeScopeForModule(moduleName);
    if (!findType)
        return;

    for (const auto classBinding : findType->getClasses().GetElements())
    {
        auto clientClassInfo = findType->findTypeDeclaredClass(classBinding->m_binary_name);
        if (!clientClassInfo)
            continue;

        auto field_hash = fnv::hash(classBinding->m_binary_name);

        for (auto i = 0; i < clientClassInfo->m_field_size; i++)
        {
            const auto field = &clientClassInfo->m_fields[i];
            if (!field)
                continue;

            auto ptr = field->m_type->getRefClass();
            auto actual_type = ptr ? ptr : field->m_type;
            auto name_hash = fnv::hash(field->m_name);

            schemaData[field_hash][name_hash].m_class = field_hash;
            schemaData[field_hash][name_hash].m_prop = name_hash;
            schemaData[field_hash][name_hash].m_offset = field->m_offset;
        }
    }
}

void Schema::init()
{
    for (auto& module : netvarModules)
        InitModule(module.c_str());
}

short Schema::get(const hash32_t& classHash, const hash32_t& propHash)
{
    return schemaData[classHash][propHash].m_offset;
}