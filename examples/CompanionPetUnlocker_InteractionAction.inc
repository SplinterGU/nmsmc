cd /Components
=GcInteractionComponentData.xml

cd /Components/=GcInteractionComponentData.xml
InteractionAction=PressButton
InteractionType=GcInteractionType.xml

cd /Components/=GcInteractionComponentData.xml/InteractionType
InteractionType=Pet

cd /Components/=GcInteractionComponentData.xml
UseIntermediateUI=False
Renderer=TkModelRendererData.xml

cd /Components/=GcInteractionComponentData.xml/Renderer
Camera=TkModelRendererCameraData.xml

cd /Components/=GcInteractionComponentData.xml/Renderer/Camera
Distance=1.8
Offset=Vector3f.xml

cd /Components/=GcInteractionComponentData.xml/Renderer/Camera/Offset
x=0.45
y=0.65
z=3

cd /Components/=GcInteractionComponentData.xml/Renderer/Camera
Roll=0
Pitch=10
Rotate=180
LightPitch=45
LightRotate=45
Wander=TkCameraWanderData.xml

cd /Components/=GcInteractionComponentData.xml/Renderer/Camera/Wander
CamWander=False
CamWanderPhase=0.003
CamWanderAmplitude=0.5

cd /Components/=GcInteractionComponentData.xml/Renderer
Fov=30
AspectRatio=1.7777
ThumbnailMode=None
FocusType=ResourceBounds
FocusLocator=
FocusOffset=Vector3f.xml

cd /Components/=GcInteractionComponentData.xml/Renderer/FocusOffset
x=0
y=0
z=0
cd /Components/=GcInteractionComponentData.xml/Renderer
FocusInterpTime=0
BlendInTime=0.8
BlendInOffset=0
Anim=
HeightOffset=0.005
UsePlayerCameraInHmd=True
AlignUIToCameraInHmd=False
UseSensibleCameraFocusNodeIsNowOffsetNode=False
LookForFocusInMasterModel=False

cd /Components/=GcInteractionComponentData.xml
RendererAlt=TkModelRendererData.xml

cd /Components/=GcInteractionComponentData.xml/RendererAlt
Camera=TkModelRendererCameraData.xml

cd /Components/=GcInteractionComponentData.xml/RendererAlt/Camera
Distance=1.5
Offset=Vector3f.xml

cd /Components/=GcInteractionComponentData.xml/RendererAlt/Camera/Offset
x=0
y=0
z=0

cd /Components/=GcInteractionComponentData.xml/RendererAlt/Camera
Roll=0
Pitch=0
Rotate=180
LightPitch=45
LightRotate=45
Wander=TkCameraWanderData.xml

cd /Components/=GcInteractionComponentData.xml/RendererAlt/Camera/Wander
CamWander=False
CamWanderPhase=0.003
CamWanderAmplitude=0.5

cd /Components/=GcInteractionComponentData.xml/RendererAlt
Fov=30
AspectRatio=1.7777
ThumbnailMode=None
FocusType=ResourceBounds
FocusLocator=
FocusOffset=Vector3f.xml

cd /Components/=GcInteractionComponentData.xml/RendererAlt/FocusOffset
x=0
y=0
z=0

cd /Components/=GcInteractionComponentData.xml/RendererAlt
FocusInterpTime=0
BlendInTime=0.8
BlendInOffset=0
Anim=
HeightOffset=0.005
UsePlayerCameraInHmd=False
AlignUIToCameraInHmd=False
UseSensibleCameraFocusNodeIsNowOffsetNode=False
LookForFocusInMasterModel=False

cd /Components/=GcInteractionComponentData.xml
Race=GcAlienRace.xml

cd /Components/=GcInteractionComponentData.xml/Race
AlienRace=None

cd /Components/=GcInteractionComponentData.xml
AttractDistanceSq=100
SecondaryMeshAlwaysVisible=False
RepeatInteraction=False
UseInteractCamera=False
BlendToCameraTime=1.5
BlendFromCameraTime=-1
ActivationCost=GcInteractionActivationCost.xml

cd /Components/=GcInteractionComponentData.xml/ActivationCost
SubstanceId=
AltIds
Cost=0
Repeat=False
RequiredTech=
UseCostID=
StartMissionOnCantAfford=
# the next only create name without value
OnlyChargeDuringSeasons

cd /Components/=GcInteractionComponentData.xml
TriggerAction=INACTIVE
TriggerActionOnPrepare=
BroadcastTriggerAction=False
InteractAngle=360
InteractDistance=100
InteractInvertFace=True
SecondaryInteractionType=GcInteractionType.xml

cd /Components/=GcInteractionComponentData.xml/SecondaryInteractionType
InteractionType=Creature

cd /Components/=GcInteractionComponentData.xml
SecondaryActivationCost=GcInteractionActivationCost.xml

cd /Components/=GcInteractionComponentData.xml/SecondaryActivationCost
SubstanceId=BAIT_BASIC
AltIds
Cost=1
Repeat=True
RequiredTech=
UseCostID=
StartMissionOnCantAfford=
OnlyChargeDuringSeasons

cd /Components/=GcInteractionComponentData.xml
EventRenderers
EventRenderersAlt
SecondaryCameraTransitionTime=1
DepthOfField=GcInteractionDof.xml

cd /Components/=GcInteractionComponentData.xml/DepthOfField
IsEnabled=True
UseGlobals=True
NearPlaneMin=2
NearPlaneAdjust=1
FarPlane=3
FarFadeDistance=2

cd /Components/=GcInteractionComponentData.xml
PuzzleMissionOverrideTable
StoryUtilityOverrideData=GcStoryUtilityOverride.xml

cd /Components/=GcInteractionComponentData.xml/StoryUtilityOverrideData
Name=
Reward=
SpecificRewardOverrideTable

cd /Components/=GcInteractionComponentData.xml

OverrideInteriorExteriorMarker=No
ReseedAfterRewardSuccess=False
UsePersonalPersistentBuffer=False
InWorldUIScaler=0
InWorldUIMinDistOverride=0
InWorldUIMinDistOverrideV2=0
InWorldUIForcedOffset=0
InWorldUIForcedOffsetV2=0
InWorldUIUseCameraUp=False
StartMissionOnUse=
AllowMissionUnderwater=True
UseUnlockedInteractionIfMaintDone=False
