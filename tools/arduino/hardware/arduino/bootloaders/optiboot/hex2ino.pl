#!/usr/bin/perl

open(HEX, "optiboot_atmega328.hex");
open(H,   ">optiboot_atmega328.h");

my $char = 0;
while( <HEX> ){
  if (/^:107..000(.*)/){
    my $hex = $1;
    while($hex=~s/^(\w\w)//){
      print H '0x';
      print H $1;
      print H ', ';
      $char++;
      if ($char == 16){
        $hex=~s/^(\w\w)//;
        $char = 0;
        print H "\n";
      }
    }
  }
}
