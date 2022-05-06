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

Texture & EntityAttributeManager::AddTexture(const Texture & texture)
{
//    attributes.emplace(EntityAttributeId::EA_Texture, new Texture(texture));
    return *dynamic_cast<Texture *>(attributes.at(EntityAttributeId::EA_Texture).get());
}

Material * EntityAttributeManager::GetMaterial()
{
    if (attributes.contains(EntityAttributeId::EA_Material))
        return dynamic_cast<Material *>(attributes.at(EntityAttributeId::EA_Material).get());
    return nullptr;
}

Texture * EntityAttributeManager::GetTexture()
{
    if (attributes.contains(EntityAttributeId::EA_Texture))
        return dynamic_cast<Texture *>(attributes.at(EntityAttributeId::EA_Material).get());
    return nullptr;
}
