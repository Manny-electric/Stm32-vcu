/*
 * This file is part of the ZombieVerter project.
 *
 * Copyright (C) 2023 Damien Maguire
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <TeslaDCDC.h>

/* This is an interface for The Tesla GEN2 DCDC converter
 * https://openinverter.org/wiki/Tesla_Model_S/X_DC/DC_Converter
 */

 void TeslaDCDC::SetCanInterface(CanHardware* c)
{
   can = c;
   can->RegisterUserMessage(0x202);
}

// Process voltage , current and temperature message from the Model s/x DCDC converter.
void TeslaDCDC::DecodeCAN(int id, uint8_t *data)
{
   if (id == 0x202)
   {
       Param::SetFloat(Param::U12V,((data[2]*256 + data[3] )*0.001));//Display 12v system voltage as read from the dcdc
       Param::SetFloat(Param::I12V,(data[4]*256 + data[5] ));//Display 12v system current as read from the dcdc
       Param::SetFloat(Param::ChgTemp,(data[2]*0.5)-40);//Display dcdc coolant temp

   }

}

void TeslaDCDC::Task100Ms() {

int opmode = Param::GetInt(Param::opmode);
//static float DCSetVal=Param::GetFloat(Param::DCSetPnt);
//static float Payload;
// Declare data frame array.
uint8_t bytes[8];

   if(opmode==MOD_RUN || opmode==MOD_CHARGE)
   {
   timer500++;
   if(timer500==5)
   {
//   Payload=(byteswap((DCSetVal-9)*146)+0x4)<<8;//prob going to not work :)
   bytes[0]=0x01;//just bodge it to 13.8v for getting running
   bytes[1]=0x00;
   bytes[2]=0x00;
   bytes[3]=0x00;
   bytes[4]=0x00;
   bytes[5]=0x00;
   bytes[6]=0x00;
   bytes[7]=0x00;
   can->Send(0x101, bytes, 8);
   timer500=0;
   }

   }


}

