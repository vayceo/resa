//
// Created on 18.07.2023.
//

#include "VehiclePipeMenu.h"
#include "CircleMenu.h"

extern CGUI *pGUI;

void CVehiclePipeMenu::Render() {

    //ImGui::ShowDemoWindow();

   // if( ImGui::IsMouseClicked( 1 ) )
    //{

    ImGui::SetNextWindowPos(pGUI->m_vecScreenCenter, ImGuiCond_Appearing);
    ImGui::OpenPopup( "PieMenu" );
    //}

    if( BeginPiePopup( "PieMenu", 1 ) )
    {
        if( PieMenuItem( "Test1" ) ) { /*TODO*/ }
        if( PieMenuItem( "Test2" ) ) { /*TODO*/ }

        if( PieMenuItem( "Test3", false ) )  { /*TODO*/ }

        if( BeginPieMenu( "Sub" ) )
        {
            if( BeginPieMenu( "Sub sub\nmenu" ) )
            {
                if( PieMenuItem( "SubSub" ) ) { /*TODO*/ }
                if( PieMenuItem( "SubSub2" ) ) { /*TODO*/ }
                EndPieMenu();
            }
            if( PieMenuItem( "TestSub" ) ) { /*TODO*/ }
            if( PieMenuItem( "TestSub2" ) ) { /*TODO*/ }
            EndPieMenu();
        }

        EndPiePopup();
    }
}