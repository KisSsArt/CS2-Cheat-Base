#pragma once

#include <cstdint>
#include <utility>
#include <vector>

#include "../Pad.h"
#include "../VirtualMethod.h"
#include "../UtlTsHash.h"

struct SchemaClassInfoData;
class SchemaSystemTypeScope;

class SchemaEnumBinding {
public:
    virtual const char* get_binding_name() = 0;
    virtual void* as_class_binding() = 0;
    virtual SchemaEnumBinding* as_enum_binding() = 0;
    virtual const char* get_binary_name() = 0;
    virtual const char* get_project_name() = 0;
public:
    char* m_binding_name; // 0x0008
};

class SchemaType {
public:
    bool getSizes(int* out_size1, uint8_t* unk_probably_not_size)
    {
        return reinterpret_cast<int(__thiscall*)(void*, int*, uint8_t*)>(m_vtable[3])(this, out_size1, unk_probably_not_size);
    }
public:
    bool getSize(int* out_size) {
        uint8_t smh = 0;
        return getSizes(out_size, &smh);
    }
public:
    uintptr_t* m_vtable; // 0x0000
    const char* m_name; // 0x0008
    SchemaSystemTypeScope* m_type_scope; // 0x0010
    uint8_t m_type_category; // ETypeCategory 0x0018
    uint8_t m_atomic_category; // EAtomicCategory 0x0019

    // find out to what class pointer points.
    SchemaType* getRefClass() 
    {
        if (m_type_category != 1)
            return nullptr;

        auto ptr = this->m_schema_type_;

        while (ptr && ptr->m_type_category == 1)
            ptr = ptr->m_schema_type_;

        return ptr;
    }

    struct array_t {
        uint32_t array_size;
        uint32_t unknown;
        SchemaType* element_type_;
    };

    struct atomic_t { // same goes for CollectionOfT
        uint64_t gap[2];
        SchemaType* template_typename;
    };

    struct atomic_tt {
        uint64_t gap[2];
        SchemaType* templates[2];
    };

    struct atomic_i {
        uint64_t gap[2];
        uint64_t integer;
    };

    // this union depends on CSchema implementation, all members above
    // is from base class ( CSchemaType )
    union // 0x020
    {
        SchemaType* m_schema_type_;
        SchemaClassInfoData* m_class_info;
        void* m_enum_binding_;
        array_t m_array_;
        atomic_t m_atomic_t_;
        atomic_tt m_atomic_tt_;
        atomic_i m_atomic_i_;
    };
};

struct SchemaClassFieldData {
    const char* m_name;// 0x0000
    SchemaType* m_type; // 0x0008
    short m_offset;    // 0x0010
    PAD(0xE);        // 0x0012
};

struct SchemaClassInfoData {
    PAD(0x8);                         // 0x0000
    const char* m_name;                 // 0x0008
    char* m_module;                     // 0x0010
    int m_size;                         // 0x0018
    __int16 m_field_size;               // 0x001C
    __int16 m_static_size;              // 0x001E
    __int16 m_metadata_size;            // 0x0020
    __int16 m_i_unk1;                   // 0x0022
    __int16 m_i_unk2;                   // 0x0024
    __int16 m_i_unk3;                   // 0x0026
    SchemaClassFieldData* m_fields;// 0x0028
};

struct SchemaType_t {
    uintptr_t* m_vtable; // 0x0000
    const char* m_name; // 0x0008
};

struct SchemaClassBinding {
    SchemaClassBinding* parent;
    const char* m_binary_name; // ex: C_World
    const char* m_module_name; // ex: libclient.so
    const char* m_class_name; // ex: client
    void* m_class_info_old_synthesized;
    void* m_class_info;
    void* m_this_module_binding_pointer;
    SchemaType_t* m_schema_type;
};

class SchemaSystemTypeScope {
public:
    __forceinline SchemaClassInfoData* findTypeDeclaredClass(const char* name)
    {
        using FindTypeDeclaredClass_t = SchemaClassInfoData* (__thiscall*)(void*, const char*);
        return VirtualMethod::getMethod<FindTypeDeclaredClass_t>(this, 2)(this, name);
    }

    __forceinline utlTsHash<SchemaClassBinding*> getClasses()
    {
        return classes;
    }

private:
    PAD(0x8); // 0x0000
    char name[256];
    PAD(0x450);
    utlTsHash<SchemaClassBinding*> classes; // 0x0558
    PAD(0x2804);
};

class SchemaSystem {
public:
    __forceinline SchemaSystemTypeScope* globalTypeScope()
    {
        using GlobalTypeScope_t = SchemaSystemTypeScope* (__thiscall*)(void*);
        return VirtualMethod::getMethod<GlobalTypeScope_t>(this, 11)(this);
    }

    __forceinline SchemaSystemTypeScope* findTypeScopeForModule(const char* module)
    {
        using FindTypeScopeForModule_t = SchemaSystemTypeScope* (__thiscall*)(void*, const char*);
        return VirtualMethod::getMethod<FindTypeScopeForModule_t>(this, 13)(this, module);
    }
};