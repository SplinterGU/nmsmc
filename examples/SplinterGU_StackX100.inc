!inputPakFile NMSARC.MetadataEtc.pak
!mbinFile METADATA/GAMESTATE/DIFFICULTYCONFIG.MBIN

# stack limit High x100

cd /InventoryStackLimitsOptionData/High

SubstanceStackLimit=999999
ProductStackLimit=999999

cd /InventoryStackLimitsOptionData/High/MaxSubstanceStackSizes

Default=999999
Personal=999999
PersonalCargo=999999
Ship=999999
ShipCargo=999999
Freighter=999999
FreighterCargo=999999
Vehicle=999999
Chest=999999
BaseCapsule=999999
MaintenanceObject=999999
UIPopup=999999

cd /InventoryStackLimitsOptionData/High/MaxProductStackSizes

Default=500
Personal=1000
PersonalCargo=1000
Ship=1000
ShipCargo=1000
Freighter=2000
FreighterCargo=2000
Vehicle=1000
Chest=2000
BaseCapsule=10000
MaintenanceObject=1000
UIPopup=100

# stack limit Normal x100

cd /InventoryStackLimitsOptionData/Normal

SubstanceStackLimit=999999
ProductStackLimit=999999

cd /InventoryStackLimitsOptionData/Normal/MaxSubstanceStackSizes

Default=50000
Personal=50000
PersonalCargo=50000
Ship=100000
ShipCargo=100000
Freighter=200000
FreighterCargo=200000
Vehicle=100000
Chest=100000
BaseCapsule=200000
MaintenanceObject=25000
UIPopup=25000

cd /InventoryStackLimitsOptionData/Normal/MaxProductStackSizes

Default=500
Personal=1000
PersonalCargo=1000
Ship=1000
ShipCargo=1000
Freighter=1000
FreighterCargo=2000
Vehicle=1000
Chest=2000
BaseCapsule=10000
MaintenanceObject=1000
UIPopup=100

# stack limit Low x100

cd /InventoryStackLimitsOptionData/Low

SubstanceStackLimit=999999
ProductStackLimit=999999

cd /InventoryStackLimitsOptionData/Low/MaxSubstanceStackSizes

Default=15000
Personal=30000
PersonalCargo=30000
Ship=30000
ShipCargo=75000
Freighter=75000
FreighterCargo=75000
Vehicle=30000
Chest=75000
BaseCapsule=125000
MaintenanceObject=15000
UIPopup=15000

cd /InventoryStackLimitsOptionData/Low/MaxProductStackSizes

Default=300
Personal=300
PersonalCargo=500
Ship=300
ShipCargo=500
Freighter=500
FreighterCargo=1000
Vehicle=300
Chest=1000
BaseCapsule=5000
MaintenanceObject=500
UIPopup=100
