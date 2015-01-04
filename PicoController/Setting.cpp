#include "Setting.h"

Setting::Setting(){
  set( "maxUpdateTime", DEF_MAX_UPDATE_TIME );
  set( "updateFreq", DEF_UPDATE_FREQ );
}

int Setting::get( String variable ){
  if ( variable == "maxUpdateTime" )
    return maxUpdateTime_;
  if ( variable == "updateFreq" )
    return updateFreq_;
  return -1;
}

bool Setting::set( String variable, int value ){
  if ( variable == "maxUpdateTime" ){
    maxUpdateTime_ = value;
    return true;
  }
  if ( variable == "updateFreq" ){
    updateFreq_ = value;
    return true;
  }
  return -1;
}
