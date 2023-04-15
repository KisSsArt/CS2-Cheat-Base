#include "Netvars.h"

#include <deque>

#include "../Interfaces.h"

#include "Source2/SchemaSystem.h"

struct netvar_offset_t {
    hash32_t m_class;
    hash32_t m_prop;
    short m_offset;
};

static std::unordered_map<hash32_t, std::unordered_map<hash32_t, netvar_offset_t>> netvarData;

void Netvar::init()
{
    std::deque<const char*> netvar_modules = { ("client.dll"), ("engine2.dll"), ("schemasystem.dll") }; // not nessecary to hardcode module names.

    for (auto& module : netvar_modules) 
    {
        auto netvar_class = interfaces->schemaSystem->findTypeScopeForModule(module);
        if (!netvar_class)
            continue;

        auto classes = netvar_class->getClasses();

        for (const auto class_binding : classes.GetElements()) 
        {
            auto client_class_info = netvar_class->findTypeDeclaredClass(class_binding->m_binary_name);
            if (!client_class_info)
                continue;

            auto field_hash = fnv::hash(class_binding->m_binary_name);

            for (auto j = 0; j < client_class_info->m_field_size; j++) {
                const auto field = &client_class_info->m_fields[j];
                if (!field)
                    continue;

                auto ptr = field->m_type->getRefClass();
                auto actual_type = ptr ? ptr : field->m_type;
                auto name_hash = fnv::hash(field->m_name);

                netvarData[field_hash][name_hash].m_class = field_hash;
                netvarData[field_hash][name_hash].m_prop = name_hash;
                netvarData[field_hash][name_hash].m_offset = field->m_offset;
            }
        }
    }
}

short Netvar::get(const hash32_t& classHash, const hash32_t& propHash)
{
    return netvarData[classHash][propHash].m_offset;
}