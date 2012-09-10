#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/kinox
mkdir -p _full/kinox/streams
piccount=0

wget --no-check-certificate "http://kinox.to" -O cache.main.list
main_list=`cat cache.main.list | tr '><' '>\n<' | grep "Stream" | grep '.html" title=' | cut -d '"' -f2 | sort -um`
skip=0
piccount=0
echo main_list $main_list
for ROUND1 in $main_list; do
	piccount=`expr $piccount + 1`
	filename=`echo $ROUND1 | sed 's!/Stream/!!'`
	picname=`echo $filename | sed 's!.html!.jpg!'`
	searchname=`echo $filename | sed 's!.html!!'`	
	
	TITLE=`echo $picname | sed 's!.jpg!!' | tr "_" " "`

	wget --no-check-certificate "http://kinox.to/$ROUND1" -O cache."$filename".list
	PIC=`cat cache."$filename".list | tr '><' '>\n<' | grep $picname | cut -d '"' -f2 | sort -um`

	hosterlist=`cat cache."$filename".list | grep $searchname | tr "=" "\n" | grep '^"Named"' | cut -d ">" -f2 | cut -d "<" -f1 | sed "s/(DivX)//"`
	echo hosterlist $hosterlist
		
	echo cache."$filename".list
	echo filename=$filename
	echo picname=$picname
	echo PIC=$PIC
	echo TITLE=$TITLE
#	exit
	URL="http://atemio.dyndns.tv/mediathek/kinox/streams/kinox.$searchname.list"

	LINE="$TITLE#$URL#$PIC#kinox_$piccount.jpg#KinoX#0"
	if [ ! -z "$TITLE" ]; then
		echo $LINE >> cache.kinox.$filename.titanlist
		echo $LINE >> cache.kinox.titanlist
	fi

	echo cache."$filename".list
	echo filename=$filename
	
	for ROUND2 in $hosterlist; do
		piccount=`expr $piccount + 1`
		echo round2 $ROUND2
		files=`cat cache."$filename".list | grep $searchname | grep Hoster= | tr ";" "\n" | grep ^Hoster= | sed 's/&/=/' | sed 's/"/=/' | grep $ROUND2 | cut -d "=" -f2`
		echo filessssssssss $files  

		if [ ! -z "$files" ];then

			PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND2.jpg"
			TITLE="$ROUND2 ($files)"	
			URL=http://$ROUND2
	
			wget --no-check-certificate "http://kinox.to/aGET/Mirror/$searchname&Hoster=$files" -O cache.$searchname.$files.list
	
			rm -rf cache.url
			cat cache.$searchname.$files.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g'
			cat cache.$searchname.$files.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g' > cache.url
cat cache.$searchname.$files.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g'
			URL=`cat cache.url`
			FILE=`echo $URL | tr "/" "\n" | tail -n1`			
			URL="$URL;$FILE;$ROUND2"
			
			echo TITLE=$TITLE
			echo URL=$URL
			echo TITLE=$TITLE
			
			echo cache.$searchname.$files.list
			if [ -z "$URL" ];then
#			URL=`cat cache.$searchname.$files.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g'`
				echo 111111111111111111111111
#				exit
				
			fi
			if [ $ROUND2 == "Sockshare.com" ] || [ $ROUND2 == "Putlocker.com" ];then		
				LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#14"
			else
				LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#15"
			fi
	
#		echo $LINE >> cache.kinox.$filename.1titanlist
			echo $LINE >> _full/kinox/streams/kinox.$searchname.list			
	
			echo files $files
		fi
#exit
	done

done

exit

cat cache.kinox.titanlist | sort -u > _full/kinox/kinox.all.list

cp -a _full/kinox /var/www/atemio/web/mediathek
exit

get id link
#wget "http://kinox.to/aGET/Mirror/Ted-1&Hoster=2" -O test1

wget http://kinox.to
cat index.html  | tr '><' '>\n<' | grep "Stream"

wget http://kinox.to//Stream/The_Dark_Knight_Rises-1.html

cat The_Dark_Knight_Rises-1.html | tr '><' '>\n<' | grep jpg


cat index.html  | tr '><' '>\n<' | grep "Stream" | grep '.html" title=' | cut -d '"' -f2 | grep Dark

cat The_Dark_Knight_Rises-1.html | tr '><' '>\n<' | grep The_Dark_Knight_Rises-1.jpg | cut -d '"' -f2

