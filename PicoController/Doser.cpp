#include "Doser.h"

Doser::Doser( Setting* settings, uint8_t doserPin ): Equip( settings, doserPin ){
  settings->init( &DOSER_MAX_DISABLE_NAME[0], &DOSER_MAX_DISABLE_DESC[0], &DOSER_MAX_DISABLE_UNIT[0], 1440 );
  settings->init( &DOSER_AMOUNT_NAME[0]     , &DOSER_AMOUNT_DESC[0]     , &DOSER_AMOUNT_UNIT[0]     , 0    );
  settings->init( &DOSER_CAL_NAME[0]        , &DOSER_CAL_DESC[0]        , &DOSER_CAL_UNIT[0]        , 35   );
  
  settings->init( &DOSER_NUM_PER_WEEK_NAME[0], &DOSER_NUM_PER_WEEK_DESC[0], 45 );
  
  maxDisableName_ = &DOSER_MAX_DISABLE_NAME[0];
}

void Doser::check( float currentTime ){
}


