#include "EntityAttributeManager.hpp"

EntityAttributeManager::EntityAttributeManager()
{
}

EntityAttributeManager::~EntityAttributeManager()
{
}

Material & EntityAttributeManager::AddMaterial()
{
    attributes[EntityAttributeId::EA_Material] = std::unique_ptr<EntityAttribute>(new Material());
    return *dynamic_cast<Material *>(attributes.at(EntityAttributeId::EA_Material).get());
}

Pbr_Material & EntityAttributeManager::AddPbrMaterial(const char * albedoMap, const char * normalMap, const char * metallicMap, const char * roughnessMap, const char * aoMap)
{
    attributes[EntityAttributeId::EA_PbrMaterial] = std::unique_ptr<EntityAttribute>(new Pbr_Material(albedoMap, normalMap, metallicMap, roughnessMap, aoMap));
    return *dynamic_cast<Pbr_Material *>(attributes.at(EntityAttributeId::EA_PbrMaterial).get());
}

Material * EntityAttributeManager::GetMaterial()
{
    if (attributes.contains(EntityAttributeId::EA_Material))
        return dynamic_cast<Material *>(attributes.at(EntityAttributeId::EA_Material).get());
    return nullptr;
}

Pbr_Material * EntityAttributeManager::GetPbrMaterial()
{
    if (attributes.contains(EntityAttributeId::EA_PbrMaterial))
        return dynamic_cast<Pbr_Material *>(attributes.at(EntityAttributeId::EA_PbrMaterial).get());
    return nullptr;
}
