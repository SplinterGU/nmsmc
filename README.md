# No Man's Sky Mod Creator (nmsmc)

This tool allows you to create mods for No Man's Sky easily without the need for coding knowledge. You only need to create definition files.

### Usage:

```sh
nmsmc [options|<definition_file>]
```

### Examples:

```sh
nmsmc SplinterGU_StackX100.def
```

##### SplinterGU_StackX100.def
```
!outputPakFile build/SplinterGU_StackX100.pak

!inputPakFile NMSARC.globals.pak
!mbinFile GCGAMEPLAYGLOBALS.GLOBAL.MBIN

# stack limit High x100

cd /DifficultyConfig/InventoryStackLimitsOptionData/High

SubstanceStackLimit=999999
ProductStackLimit=999999

cd /DifficultyConfig/InventoryStackLimitsOptionData/High/MaxSubstanceStackSizes

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

cd /DifficultyConfig/InventoryStackLimitsOptionData/High/MaxProductStackSizes

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

cd /DifficultyConfig/InventoryStackLimitsOptionData/Normal

SubstanceStackLimit=999999
ProductStackLimit=999999

cd /DifficultyConfig/InventoryStackLimitsOptionData/Normal/MaxSubstanceStackSizes

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

cd /DifficultyConfig/InventoryStackLimitsOptionData/Normal/MaxProductStackSizes

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

cd /DifficultyConfig/InventoryStackLimitsOptionData/Low

SubstanceStackLimit=999999
ProductStackLimit=999999

cd /DifficultyConfig/InventoryStackLimitsOptionData/Low/MaxSubstanceStackSizes

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

cd /DifficultyConfig/InventoryStackLimitsOptionData/Low/MaxProductStackSizes

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
```

### Options:
- `-h, --help` :        Show this help message and exit.
- `-V, --version` :     Show version information.


### How to Build:

To build the NMS Mod Creator, you can follow these steps:

1. Clone the repository from GitHub:

   ```sh
   git clone https://github.com/SplinterGU/nmsmc.git
   cd nmsmc
   ```

2. Create a build directory and navigate to it:

   ```sh
   mkdir build
   cd build
   ```

3. Generate the build files using CMake:

   ```sh
   cmake ..
   ```

4. Build the NMS Mod Creator:

   ```sh
   make
   ```

### Installation:

Before using NMS Mod Creator, ensure you meet the following requirements:

- libxml2
- [psar](https://github.com/SplinterGU/PSARc)
- [MBINCompiler](https://github.com/monkeyman192/MBINCompiler)

Make sure that the following tools are available in your system's PATH:

- psar
- MBINCompiler

You can install these tools by following the instructions in their respective repositories.

### License

This software is provided under the terms of the MIT License. You are free to use, modify, and distribute this software, subject to the conditions and limitations of the MIT License. For more details, please see the LICENSE file included with this software.

### Contact and Support

- Report bugs to: splintergu@gmail.com
- Home page: [PSARc on GitHub](https://github.com/SplinterGU/nmsmc)
