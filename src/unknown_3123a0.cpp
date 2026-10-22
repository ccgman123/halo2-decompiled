// @flags /O2 /GL-
// Havok was built without LTCG; use its flags from docs/DECOMPILING.md.
#include "unknown_1cec30.h"
#include <stddef.h>

typedef char assert_property_word_size[(sizeof(s_havok_property_word) == 4) ? 1 : -1];
typedef char assert_property_record_size[(sizeof(s_havok_entity_property) == 8) ? 1 : -1];
typedef char assert_property_value_offset[(offsetof(s_havok_entity_property, m_value) == 4) ? 1 : -1];
typedef char assert_entity_properties_offset[(offsetof(hkEntity, m_properties) == 0x30) ? 1 : -1];
typedef char assert_entity_property_count_offset[(offsetof(hkEntity, m_property_count) == 0x34) ? 1 : -1];

// Remove the first matching property, preserving the order of later records.
// @retail 0x3123a0
s_havok_property_word hkEntity::entity_property_erase(dword key)
{
    // One-time read so the search bound is not merged with the later member update.
    long count = *(long volatile *)&m_property_count;
    for (long i = 0; i < count; ++i)
    {
        if (m_properties[i].m_key == key)
        {
            s_havok_entity_property removed = m_properties[i];
            --m_property_count;
            for (long j = i; j < m_property_count; ++j)
            {
                m_properties[j] = m_properties[j + 1];
            }
            return removed.m_value;
        }
    }
    return s_havok_property_word(0);
}
