#-------------------------------------------------
#
# Project created by QtCreator 2015-07-02T22:51:41
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    Sources/gamewidget.cpp \
    Sources/GameStates/gamestate.cpp \
    Sources/UserInterface/uielement.cpp \
    Sources/UserInterface/HUD/hudelement.cpp \
    Sources/UserInterface/Screen/scene.cpp \
    Sources/UserInterface/Screen/camera.cpp \
    Sources/GameObjects/entity.cpp \
    Sources/GameObjects/actor.cpp \
    Sources/GameObjects/Characters/player.cpp \
    Sources/GameObjects/Characters/bot.cpp \
    Sources/Process/gamemod.cpp \
    Box2D/Collision/Shapes/b2ChainShape.cpp \
    Box2D/Collision/Shapes/b2CircleShape.cpp \
    Box2D/Collision/Shapes/b2EdgeShape.cpp \
    Box2D/Collision/Shapes/b2PolygonShape.cpp \
    Box2D/Collision/b2BroadPhase.cpp \
    Box2D/Collision/b2CollideCircle.cpp \
    Box2D/Collision/b2CollideEdge.cpp \
    Box2D/Collision/b2CollidePolygon.cpp \
    Box2D/Collision/b2Collision.cpp \
    Box2D/Collision/b2Distance.cpp \
    Box2D/Collision/b2DynamicTree.cpp \
    Box2D/Collision/b2TimeOfImpact.cpp \
    Box2D/Common/b2BlockAllocator.cpp \
    Box2D/Common/b2Draw.cpp \
    Box2D/Common/b2Math.cpp \
    Box2D/Common/b2Settings.cpp \
    Box2D/Common/b2StackAllocator.cpp \
    Box2D/Common/b2Timer.cpp \
    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    Box2D/Dynamics/Contacts/b2Contact.cpp \
    Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    Box2D/Dynamics/Joints/b2GearJoint.cpp \
    Box2D/Dynamics/Joints/b2Joint.cpp \
    Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    Box2D/Dynamics/b2Body.cpp \
    Box2D/Dynamics/b2ContactManager.cpp \
    Box2D/Dynamics/b2Fixture.cpp \
    Box2D/Dynamics/b2Island.cpp \
    Box2D/Dynamics/b2World.cpp \
    Box2D/Dynamics/b2WorldCallbacks.cpp \
    Box2D/Rope/b2Rope.cpp \
    SOIL/image_DXT.c \
    SOIL/image_helper.c \
    SOIL/SOIL.c \
    SOIL/stb_image_aug.c \
    Sources/UserInterface/textures.cpp \
    Sources/GameObjects/gameobject.cpp \
    Sources/GameObjects/noninteractive.cpp \
    Sources/GameObjects/DynamicObjects/dynamicobject.cpp \
    Sources/GameObjects/Characters/npc.cpp \
    Sources/GameObjects/InteractiveObjects/interactiveobject.cpp \
    Sources/GameObjects/InteractiveObjects/ladder.cpp \
    Sources/GameObjects/Characters/bodypart.cpp\
    Sources/GameObjects/Rooms/room.cpp \
    Sources/GameObjects/Builds/build.cpp \
    Sources/UserInterface/displaydata.cpp \
    Sources/UserInterface/keylinedata.cpp \
    Sources/UserInterface/texturedata.cpp \
    Sources/UserInterface/color.cpp \
    Sources/GameObjects/InteractiveObjects/Vehicles/vehicle.cpp \
    Sources/GameObjects/InteractiveObjects/Vehicles/car.cpp \
    Sources/UserInterface/nondrawable.cpp \
    Sources/userdata.cpp \
    Sources/GameObjects/Characters/bodyparts.cpp \
    Sources/generalinfo.cpp \
    Sources/GameObjects/Sensors/groundsensor.cpp \
    Box2D/Json/b2dJson.cpp \
    Box2D/Json/b2dJsonImage.cpp \
    Box2D/Json/b2dJsonImage_OpenGL.cpp \
    Box2D/Json/jsoncpp.cpp \
    Sources/GameObjects/entitylist.cpp \
    Sources/GameObjects/enum.cpp \
    Sources/triangulate.cpp \
    poly2tri/common/shapes.cc \
    poly2tri/sweep/advancing_front.cc \
    poly2tri/sweep/cdt.cc \
    poly2tri/sweep/sweep.cc \
    poly2tri/sweep/sweep_context.cc \
    Sources/UserInterface/triangletexturedata.cpp

HEADERS  += \
    Sources/gamewidget.h \
    Sources/gameinstance.h \
    Sources/GameStates/gamestate.h \
    Sources/UserInterface/uielement.h \
    Sources/UserInterface/HUD/hudelement.h \
    Sources/UserInterface/Screen/scene.h \
    Sources/UserInterface/Screen/camera.h \
    Sources/GameObjects/entity.h \
    Sources/GameObjects/actor.h \
    Sources/GameObjects/Characters/player.h \
    Sources/GameObjects/Characters/bot.h \
    Sources/Process/gamemod.h \
    Box2D/Collision/Shapes/b2ChainShape.h \
    Box2D/Collision/Shapes/b2CircleShape.h \
    Box2D/Collision/Shapes/b2EdgeShape.h \
    Box2D/Collision/Shapes/b2PolygonShape.h \
    Box2D/Collision/Shapes/b2Shape.h \
    Box2D/Collision/b2BroadPhase.h \
    Box2D/Collision/b2Collision.h \
    Box2D/Collision/b2Distance.h \
    Box2D/Collision/b2DynamicTree.h \
    Box2D/Collision/b2TimeOfImpact.h \
    Box2D/Common/b2BlockAllocator.h \
    Box2D/Common/b2Draw.h \
    Box2D/Common/b2GrowableStack.h \
    Box2D/Common/b2Math.h \
    Box2D/Common/b2Settings.h \
    Box2D/Common/b2StackAllocator.h \
    Box2D/Common/b2Timer.h \
    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    Box2D/Dynamics/Contacts/b2CircleContact.h \
    Box2D/Dynamics/Contacts/b2Contact.h \
    Box2D/Dynamics/Contacts/b2ContactSolver.h \
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2PolygonContact.h \
    Box2D/Dynamics/Joints/b2DistanceJoint.h \
    Box2D/Dynamics/Joints/b2FrictionJoint.h \
    Box2D/Dynamics/Joints/b2GearJoint.h \
    Box2D/Dynamics/Joints/b2Joint.h \
    Box2D/Dynamics/Joints/b2MotorJoint.h \
    Box2D/Dynamics/Joints/b2MouseJoint.h \
    Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    Box2D/Dynamics/Joints/b2PulleyJoint.h \
    Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    Box2D/Dynamics/Joints/b2RopeJoint.h \
    Box2D/Dynamics/Joints/b2WeldJoint.h \
    Box2D/Dynamics/Joints/b2WheelJoint.h \
    Box2D/Dynamics/b2Body.h \
    Box2D/Dynamics/b2ContactManager.h \
    Box2D/Dynamics/b2Fixture.h \
    Box2D/Dynamics/b2Island.h \
    Box2D/Dynamics/b2TimeStep.h \
    Box2D/Dynamics/b2World.h \
    Box2D/Dynamics/b2WorldCallbacks.h \
    Box2D/Rope/b2Rope.h \
    Box2D/Box2D.h \
    SOIL/image_DXT.h \
    SOIL/image_helper.h \
    SOIL/SOIL.h \
    SOIL/stb_image_aug.h \
    SOIL/stbi_DDS_aug.h \
    SOIL/stbi_DDS_aug_c.h \
    Sources/UserInterface/textures.h \
    Sources/GameObjects/gameobject.h \
    Sources/GameObjects/noninteractive.h \
    Sources/GameObjects/Rooms/room.h \
    Sources/GameObjects/DynamicObjects/dynamicobject.h \
    Sources/GameObjects/Characters/npc.h \
    Sources/GameObjects/InteractiveObjects/interactiveobject.h \
    Sources/GameObjects/InteractiveObjects/ladder.h \
    Sources/GameObjects/Characters/bodypart.h\
    Sources/GameObjects/Builds/build.h \
    Sources/UserInterface/displaydata.h \
    Sources/UserInterface/Screen/keylinedata.h \
    Sources/UserInterface/keylinedata.h \
    Sources/UserInterface/texturedata.h \
    Sources/UserInterface/color.h \
    Sources/GameObjects/InteractiveObjects/Vehicles/vehicle.h \
    Sources/GameObjects/InteractiveObjects/Vehicles/car.h \
    Sources/UserInterface/nondrawable.h \
    Sources/userdata.h \
    Sources/GameObjects/Characters/bodyparts.h \
    Sources/GameObjects/entitylist.h \
    Sources/generalinfo.h \
    Sources/GameObjects/enum.h \
    Sources/GameObjects/Sensors/groundsensor.h \
    Box2D/Json/json/json-forwards.h \
    Box2D/Json/json/json.h \
    Box2D/Json/b2dJson.h \
    Box2D/Json/b2dJsonImage.h \
    Box2D/Json/b2dJsonImage_OpenGL.h \
    Box2D/Json/bitmap.h \
    poly2tri/common/shapes.h \
    poly2tri/common/utils.h \
    poly2tri/sweep/advancing_front.h \
    poly2tri/sweep/cdt.h \
    poly2tri/sweep/sweep.h \
    poly2tri/sweep/sweep_context.h \
    poly2tri/poly2tri.h \
    Sources/triangulate.h \
    Sources/UserInterface/triangletexturedata.h


unix|win32: LIBS += -L$$PWD/SOIL/ -llibSOIL

INCLUDEPATH += $$PWD/SOIL
DEPENDPATH += $$PWD/SOIL

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/SOIL/libSOIL.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/SOIL/libSOIL.a

