#include "ItemManager.h"
#include "RAFManager.h"
#include "Inibin.h"

#include <string>

using namespace std;

ItemManager* ItemManager::instance = 0;

void ItemManager::init() {
   // TODO : this is highly inefficient
   std::vector<unsigned char> iniFile;
   
   for(uint32 i = 1000; i < 4000; ++i) {
      iniFile.clear();
      if(!RAFManager::getInstance()->readFile("DATA/items/"+to_string(i)+".inibin", iniFile)) {
         continue;
      }

      Inibin inibin(iniFile);    
      
      uint32 maxStack = inibin.getIntValue("DATA", "MaxStack");
      uint32 price = inibin.getIntValue("DATA", "Price");
      
      float sellBack = 0.7f;
      
      if(inibin.keyExists("DATA", "SellBackModifier")) {
         sellBack = inibin.getFloatValue("DATA", "SellBackModifier");
      }
      
      vector<StatMod> statMods;
      
      statMods.push_back({MM_Two, FM2_Bonus_Ad_Flat, inibin.getFloatValue("DATA", "FlatPhysicalDamageMod")});
      statMods.push_back({MM_Two, FM2_Bonus_Ad_Pct, inibin.getFloatValue("DATA", "PercentPhysicalDamageMod")});
      statMods.push_back({MM_Two, FM2_Bonus_Ap_Flat, inibin.getFloatValue("DATA", "FlatMagicDamageMod")});
      statMods.push_back({MM_Two, FM2_Hp5, inibin.getFloatValue("DATA", "FlatHPRegenMod")});
      statMods.push_back({MM_Two, FM2_Crit_Chance, inibin.getFloatValue("DATA", "FlatCritChanceMod")});
      statMods.push_back({MM_Two, FM2_Armor, inibin.getFloatValue("DATA", "FlatArmorMod")});
      statMods.push_back({MM_Two, FM2_Magic_Armor, inibin.getFloatValue("DATA", "FlatSpellBlockMod")});
      
      statMods.push_back({MM_Four, FM4_MaxHp, inibin.getFloatValue("DATA", "FlatHPPoolMod")});
      statMods.push_back({MM_Four, FM4_MaxMp, inibin.getFloatValue("DATA", "FlatMPPoolMod")});
      statMods.push_back({MM_Four, FM4_Speed, inibin.getFloatValue("DATA", "FlatMovementSpeedMod")});
      
      itemTemplates[i] = new ItemTemplate(i, maxStack, price, sellBack, statMods);
   }
   
   printf("Loaded %lu items\n", itemTemplates.size());
}

const ItemTemplate* ItemManager::getItemTemplateById(uint32 id) {
   if(itemTemplates.find(id) == itemTemplates.end()) {
      return 0;
   }
   
   return itemTemplates[id];
}