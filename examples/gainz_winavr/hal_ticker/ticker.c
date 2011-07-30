/*******************************************************************************
 * This file is part of OpenWSN, the Open Wireless Sensor Network Platform.
 *
 * Copyright (C) 2005-2010 zhangwei(TongJi University)
 *
 * OpenWSN is a free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 or (at your option) any later version.
 *
 * OpenWSN is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * For non-opensource or commercial applications, please choose commercial license.
 * Refer to OpenWSN site http://code.google.com/p/openwsn/ for more detail.
 *
 * For other questions, you can contact the author through email openwsn#gmail.com
 * or the mailing address: Dr. Wei Zhang, Dept. of Control, Dianxin Hall, TongJi
 * University, 4800 Caoan Road, Shanghai, China. Zip: 201804
 *
 ******************************************************************************/
#include "../../common/openwsn/hal/hal_configall.h"
#include "../../common/openwsn/hal/hal_foundation.h"
#include "../../common/openwsn/hal/hal_cpu.h"
#include "../../common/openwsn/hal/hal_debugio.h"
#include "../../common/openwsn/hal/hal_uart.h"
#include "../../common/openwsn/hal/hal_led.h"
#include "../../common/openwsn/hal/hal_assert.h"
#include "../../common/openwsn/hal/hal_targetboard.h"
#include "../../common/openwsn/hal/hal_ticker.h"

#define interval 50
static volatile uint8 g_count=0;
static TiSysTimer g_systimer;
void on_timer_expired( void * object, TiEvent * e );
	
int main()
{
	target_init();
	HAL_SET_PIN_DIRECTIONS();
	wdt_disable();//watch dog disable
	led_on( LED_ALL );
	hal_delay( 500 );
	led_off( LED_ALL );
	dbo_open(38400);


    TiSysTimer * systimer;
	systimer =  systm_construct( (void *)&g_systimer, sizeof(g_systimer) );

    //todo 在systm_open函数中并没有把interval的值赋给相应的寄存器，要想改变计数时间必须到
	//hal_ticker.c文件中改变 SYSTM_HOPEINTERVAL宏的值。
	systimer = systm_open( systimer,  interval, on_timer_expired,NULL );

	hal_assert( systimer!=NULL);
    
    hal_enable_interrupts();
	//rtc_start( rtc );
	//systm_start( systimer );
	while(1)
	{
		
	}

	
	}
	
/*
	// if not use listener 
	while (!rtc_expired(rtc))
	{
		//do something;
	}
*/
void on_timer_expired( void * object, TiEvent * e )
        {   	
			/*
			g_count ++;
               if (g_count == 1)
                 {
	              led_toggle( LED_GREEN );
	              g_count = 0;

                  }*/
			led_toggle( LED_GREEN );
			dbc_putchar(0xfe);
			
         }      

