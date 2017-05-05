#!/bin/sh

if [ "$4" == "titan" ]; then
	LOCKFILE=/tmp/lock.txt
	if [ -e ${LOCKFILE} ] && kill -0 `cat ${LOCKFILE}`; then
		echo "already running"
		exit
	fi
	trap "rm -f ${LOCKFILE}; exit" INT TERM EXIT
	echo $$ > ${LOCKFILE}
	input1=$1
	streamport=$2
	titanservice=$3
	pngfile="/tmp/vdump.png"
	vmode=`cat /proc/stb/video/videomode`
	stream_aspect="undef"
	
	rm -rf /var/usr/local/share/titan/web/tmp
	ln -sn /tmp /var/usr/local/share/titan/web/tmp
	
#	mkdir -p /var/usr/local/share/titan/web/tmp
	
	echo titanservice $titanservice
	
	if [ "$input1" == "3" ]; then
		rm -f "$pngfile"
# check mode
		if [ "$vmode" == "576p50" ] || [ "$vmode" == "576i50" ] || [ "$vmode" == "pal" ]; then
			echo "[grab.sh] videomod = $vmode"
			stream_aspect=`cat /proc/stb/vmpeg/0/aspect`
		fi
  
# erzeuge tv bild
		ffmpeg -itsoffset -4 -i "http://127.0.0.1:"$streamport"/"$titanservice -vframes 1 -vcodec png -sn -an -y -f image2 "$pngfile"
		mv /tmp/dump.png /tmp/vdump.png
		echo "[grab.sh] used video grab file: $pngfile"
		
		# merge tv bild mit osd bild
		if [ "$stream_aspect" == "1" ]; then
			echo "[grab.sh] convert to 16:9"
			grab -r 1024 -i 576 -p -f "$pngfile"
		else
			grab -p -f "$pngfile"
		fi
    
		rm -f "$pngfile"
		mv /tmp/screenshot.png /var/usr/local/share/titan/web/tmp/dump.png
  
	else 
		if [ "$input1" == "2" ]; then
			ffmpeg -itsoffset -4 -i http://127.0.0.1:22222/$titanservice -vframes 1 -vcodec png -sn -an -y -f image2 /tmp/vdump.png
			mv /tmp/vdump.png /var/usr/local/share/titan/web/tmp/dump.png
		else 
			if [ "$input1" == "1" ]; then
				if [ "$vmode" == "576p50" ] || [ "$vmode" == "576i50" ] || [ "$vmode" == "pal" ]; then
					stream_aspect=`cat /proc/stb/video/aspect`
					if [ "$stream_aspect" == "16:9" ]; then
						echo "[grab.sh] convert to 16:9"
						/sbin/grab -o -j 40 -r 720 -i 405
					else
						/sbin/grab -o -j 40 -r 720 -i 576
					fi
				else 
					/sbin/grab -o -j 40 -r 960
				fi
				mv /tmp/screenshot.jpg /tmp/dump.jpg
			fi
	    fi
	fi
	rm -f ${LOCKFILE}
fi
