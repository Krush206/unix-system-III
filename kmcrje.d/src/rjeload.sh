#
#	Rjeload uses the lines file to find the KMC to be loaded
#	and started, then execs the appropriate rje?init.
#	Mode should be 500 (owner rje).
#

LINES=/usr/rje/lines
NAME=`expr //$0 : '.*/\(.*\)load'`

#	*** Loop until proper line is found ***

while read line
do
	set $line

	#	*** $3 is the directory	***
	#	*** $4 is the prefix	***
	#	*** $5 is the device	***

	if [ "$NAME" = "$4" ]
	then
		cd $3
		echo "$NAME: Devices are: /dev/$5, \c"
		set `ls -l /dev/$5`

		#	*** $6 is the VPM minor device number	***
		#	*** The KMC device is the first 2 bits	***

		KMC="/dev/kmc`expr $6 / 64`"
		echo "$KMC"
		kasb -d $KMC <<-!
		.reset
		.load "/etc/rjeproto"
		.run 6
		!
		./${NAME}init
		exit 1
	fi
done < $LINES
if [ $? -eq 0 ]
then
	echo "Can't find $NAME in $LINES"
	exit 1
fi
