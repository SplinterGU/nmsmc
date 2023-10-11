!inputPakFile NMSARC.globals.pak

!mbinFile GCWATERGLOBALS.GLOBAL.MBIN

cd /WaterHeavyAirColour
A=0.01

cd /WaterDataDay/HeavyAir/Colours/*/Colour1
A=0.025

cd /WaterDataDay/HeavyAir/Colours/*/Colour2
A=0.025

#cd /WaterColourNight/WaterFogColourNear
#A=0.025
#cd /WaterColourNight/WaterFogColourFar
#A=0.025
#cd /WaterColourNight/WaterColourBase
#A=0.025
#cd /WaterColourNight/WaterColourAdd
#A=0.025
#cd /WaterColourNight/FoamColour
#A=0.025

cd /WaterColourNight/*
A=0.025

# First Property element on /WaterColourNight
#cd /WaterColourNight/1
#A=0.025
# Second Property element on /WaterColourNight
#cd /WaterColourNight/2
#A=0.025
