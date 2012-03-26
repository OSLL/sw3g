#!/bin/sh
iwlist scanning 2>/dev/null | sed -n '
'/ESSID/' !{
           '/Cell/' !{
                     d
           }

	   '/Cell/'  {
	             s/^[ ]*//g
		     s/Cell [0-9]\+ - Address: //g
		     x
		     d
	   }
}
'/ESSID/' {
	  s/^[ ]*//g
  	  s/ESSID:\"\(.*\)\"/\1/g	
	  p
	  x
	  p
}'

