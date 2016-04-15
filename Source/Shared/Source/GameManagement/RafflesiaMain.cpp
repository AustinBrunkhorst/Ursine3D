#include "Precompiled.h"

#include "RafflesiaMain.h"

using namespace ursine;

RafflesiaMain::RafflesiaMain(void)
{

}

RafflesiaMain::~RafflesiaMain(void)
{
    printf( "destruct\n" );
}

void RafflesiaMain::OnInitialize(GameContext *context, const Json &configObj)
{
    GameManager::OnInitialize( context, configObj );

    printf( "AYYY BOYES\n" );
}