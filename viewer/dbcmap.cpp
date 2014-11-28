#include "dbcmap.h"

DBCMap::DBCMap() : AbstractDBCFormat(Cataclysm)
{
   this->addField(Integer,    "ID");
   this->addField(String,     "Directory");
   this->addField(Integer,    "InstanceType");
   this->addField(Integer,    "Flags");
   this->addField(Integer,    "MapType");
   this->addField(Integer,    "Unknown1");
   this->addField(Localized,  "MapName");
   this->addField(Integer,    "AreaTableId");
   this->addField(Localized,  "DescriptionAlliance");
   this->addField(Localized,  "DescriptionHorde");
   this->addField(Integer,    "LoadingScreenId");
   this->addField(Float,      "MinimapIconScale");
   this->addField(Integer,    "CorpseMapId");
   this->addField(Float,      "CorpseX");
   this->addField(Float,      "CorpseY");
   this->addField(Integer,    "TimeOfDayOverride");
   this->addField(Integer,    "ExpanseionId");
   this->addField(Integer,    "RaidOffset");
   this->addField(Integer,    "MaxPlayers");
   this->addField(Integer,    "ParentMapId");
   this->addField(Integer,    "CosmeticParentMapId");
   this->addField(Integer,    "TimeOffset");
}
