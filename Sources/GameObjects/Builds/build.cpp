#include "build.h"

Build::Build(Textures* texture_p, b2World* world)
{
   this->texture_p=texture_p;
   this->world=world;

   downDoorRooms = new FactoryDownDoorRoom;
   leftDoorRooms = new FactoryLeftDoorRoom;
   rightDoorRooms = new FactoryRightDoorRoom;
   upDoorRooms = new FactoryUpDoorRoom;
   leftAndRightDoorRooms = new FactoryLeftAndRightDoorRoom;
   upAndDownDoorRooms = new FactoryUpAndDownDoorRoom;

}



void Build::generationDangeon(b2Vec2 center, int maxNumberRoomInHeight, int maxNumberRoomInWidth)
{
    bool isExit;
    int end;
    planBuild=new TypeRoom *[maxNumberRoomInHeight];
    for(int i=0;i<maxNumberRoomInHeight;i++){
        planBuild[i] = new TypeRoom[maxNumberRoomInWidth];
        for(int j=0;j<maxNumberRoomInWidth;j++){
            planBuild[i][j]=EMPTY;
        }
    }
    planBuild[0][3]=DOWN_DOOR;
    for(int i=1;i<maxNumberRoomInHeight;i++){
        for(int j=0;j<maxNumberRoomInWidth;j++){
            if(planBuild[i-1][j]==DOWN_DOOR || planBuild[i-1][j]==UP_AND_DOWN_DOOR )
            {
                if(i!=maxNumberRoomInHeight-1){
                    switch(rand()%3){
                    case 0:
                        planBuild[i][j]=UP_AND_DOWN_DOOR;
                        break;
                    case 1:
                    case 2:
                        planBuild[i][j]=UP_DOOR;
                        break;
                    }
                }
                else{
                    planBuild[i][j]=UP_DOOR;
                }


            }
        }
        for(int j=0;j<maxNumberRoomInWidth;j++){
            if(planBuild[i][j]==EMPTY){
                if(j!=0 && j!=maxNumberRoomInWidth-1){
                   if((planBuild[i][j-1]==EMPTY  || planBuild[i][j-1]==LEFT_DOOR) && planBuild[i][j+1]==EMPTY && j+2<=maxNumberRoomInWidth){
                       switch (rand()%2) {
                       case 0:
                           planBuild[i][j]=EMPTY;
                           break;
                       case 1:
                           planBuild[i][j]=RIGHT_DOOR;
                           break;
                       }
                   }
                   else{
                       if(planBuild[i][j-1]==EMPTY && planBuild[i][j+1]!=EMPTY && planBuild[i][j+1]!=RIGHT_DOOR){
                           planBuild[i][j]=RIGHT_DOOR;
                       }
                       if(planBuild[i][j-1]!=EMPTY && planBuild[i][j-1]!=LEFT_DOOR && i!=maxNumberRoomInHeight-1 )
                       {
                           isExit=false;
                           end=0;
                           for(int k=j;k<0;k--){
                               if(planBuild[i][k]==RIGHT_DOOR) {
                                   end=k;
                                   break;
                               }
                           }
                           for(int k=end;k<j;k++){
                               if(planBuild[i][k]==DOWN_DOOR  || planBuild[i][k]==UP_AND_DOWN_DOOR){
                                   isExit=true;
                               }
                           }
                           if(isExit){
                               switch (rand()%3) {
                               case 0:

                                   planBuild[i][j]=DOWN_DOOR;
                                   break;
                               case 1:
                                   planBuild[i][j]=LEFT_AND_RIGHT_DOOR;
                                   break;
                               case 2:
                                   planBuild[i][j]=LEFT_DOOR;
                                   break;
                               }
                           }
                           else{
                               switch (rand()%2) {
                               case 0:
                                   planBuild[i][j]=DOWN_DOOR;
                                   break;
                               case 1:
                                   planBuild[i][j]=LEFT_AND_RIGHT_DOOR;
                                   break;
                               }
                           }
                       }
                       if(j+2>maxNumberRoomInWidth && i!=maxNumberRoomInHeight-1){
                           for(int k=j;k<0;k--){
                               if(planBuild[i][k]==RIGHT_DOOR) {
                                   end=k;
                                   break;
                               }
                           }
                           for(int k=end;k<j;k++){
                               if(planBuild[i][k]==DOWN_DOOR || planBuild[i][k]==UP_AND_DOWN_DOOR){
                                   isExit=true;
                               }
                           }
                           if(isExit){
                               switch (rand()%3) {
                               case 0:
                                   planBuild[i][j]=DOWN_DOOR;
                                   break;
                               case 1:
                                   planBuild[i][j]=LEFT_AND_RIGHT_DOOR;
                                   break;
                               case 2:
                                   planBuild[i][j]=LEFT_DOOR;
                                   break;
                               }
                           }
                           else{
                               planBuild[i][j]=DOWN_DOOR;
                           }
                       }
                       if(i==maxNumberRoomInHeight-1){
                           if(planBuild[i][j-1]!=EMPTY && planBuild[i][j-1]!=LEFT_DOOR)
                           {
                               isExit=false;
                               end=0;
                               for(int k=j;k<0;k--){
                                   if(planBuild[i][k]==RIGHT_DOOR) {
                                       end=k;
                                       break;
                                   }
                               }
                               for(int k=end;k<j;k++){
                                   if(planBuild[i][k]==UP_DOOR){
                                       isExit=true;
                                   }
                               }
                               if(isExit){
                                   switch (rand()%2) {
                                   case 0:
                                       planBuild[i][j]=LEFT_AND_RIGHT_DOOR;
                                       break;
                                   case 1:
                                       planBuild[i][j]=LEFT_DOOR;
                                       break;
                                   }
                               }
                               else{
                                   planBuild[i][j]=LEFT_AND_RIGHT_DOOR;
                               }
                           }
                           if(j+2>maxNumberRoomInWidth){
                               switch (rand()%2) {
                               case 0:
                                   planBuild[i][j]=LEFT_AND_RIGHT_DOOR;
                                   break;
                               case 1:
                                   planBuild[i][j]=LEFT_DOOR;
                                   break;
                               }
                           }
                       }
                   }
                }
                else{
                    if(j==0){
                        switch (rand()%2) {
                        case 0:
                            planBuild[i][j]=EMPTY;
                            break;
                        case 1:
                            planBuild[i][j]=RIGHT_DOOR;
                            break;
                        }
                    }
                    if(j==maxNumberRoomInWidth-1 && planBuild[i][j-1]!=EMPTY && planBuild[i][j-1]!=RIGHT_DOOR ){
                        planBuild[i][j]=RIGHT_DOOR;
                        }
                    else{
                        planBuild[i][j]=EMPTY;
                    }

                }
            }
        }
    }
}
