#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/kinox
mkdir -p _full/kinox/streams
piccount=0
main_list="Cine-Films Popular-Movies Popular-Documentations Popular-Series Latest-Documentations Latest-Series"

for ROUND0 in $main_list; do
	wget --no-check-certificate "http://kinox.to/$ROUND0.html" -O cache.main.next.list

	main_next_list=`cat cache.main.next.list | grep /Stream/ | sed 's!/Stream/!\n/Stream/!' | grep ^/Stream/ | cut -d '"' -f1  | cut -d "'" -f1 | sort -um`
	
	skip=0
	piccount=0
	echo main_next_list $main_next_list
#	exit


	TITLE=`echo $ROUND0 | tr "_" " " | tr "-" " "`
	piccount=`expr $piccount + 1`
	URL="http://atemio.dyndns.tv/mediathek/kinox/streams/kinox.$ROUND0.list"
	PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND0.jpg"
	LINE="$TITLE#$URL#$PIC#kinox_$piccount.jpg#KinoX#0"
	if [ ! -z "$TITLE" ]; then
		echo $LINE >> cache.kinox.category.titanlist	
	fi
		
	for ROUND1 in $main_next_list; do
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
#			files=`cat cache."$filename".list | grep $searchname | grep Hoster= | tr ";" "\n" | grep ^Hoster= | sed 's/&/=/' | sed 's/"/=/' | grep $ROUND2 | cut -d "=" -f2`
			files=`cat cache."$filename".list | grep $searchname | grep $ROUND2 | tr ' ' '\n' | grep ^rel= | cut -d '"' -f2 | sed "s/amp;//" | sed "s/amp;//" | sed "s/amp;//" | sed "s/amp;//"`
	
			if [ ! -z "$files" ];then
	
				PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND2.jpg"
#				TITLE="$ROUND2 ($files)"	
				TITLE="$ROUND2"
				URL=http://$ROUND2

				wget --no-check-certificate "http://kinox.to/aGET/Mirror/$files" -O cache.$searchname.$ROUND2.list
		
				rm -rf cache.url
				cat cache.$searchname.$ROUND2.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g'

				cat cache.$searchname.$ROUND2.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g' > cache.url

				URL=`cat cache.url`
				FILE=`echo $URL | tr "/" "\n" | tail -n1`			
				URL="$URL;$FILE;$ROUND2"
				
				echo TITLE=$TITLE
				echo URL=$URL
				echo TITLE=$TITLE
				
				#echo cache.$searchname.$files.list
				echo cache.$searchname.$ROUND2.list
				
				
				if [ -z "$URL" ];then
	#			URL=`cat cache.$searchname.$files.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g'`
					echo 111111111111111111111111
	#				exit
					
				fi
				if [ $ROUND2 == "Sockshare.com" ] || [ $ROUND2 == "Putlocker.com" ];then		
					LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#14"
				elif [ $ROUND2 == "FileNuke.com" ];then		
					LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#15"
				else
					LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#16"
				fi
				echo $LINE >> _full/kinox/streams/kinox.$searchname.list			
			fi
		done
		cat cache.kinox.$filename.titanlist | sort -u >> _full/kinox/streams/kinox.$ROUND0.list	
	done
done

cat cache.kinox.titanlist | sort -u > _full/kinox/kinox.all-sorted.list
cat cache.kinox.category.titanlist | sort -u > _full/kinox/kinox.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	if [ `cat cache.kinox.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.kinox.titanlist | grep ^"$ROUND" > cache.kinox.titanlist."$ROUND"
		cat cache.kinox.titanlist."$ROUND" | sort -um > _full/kinox/streams/kinox.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/kinox/streams/kinox."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#KinoX#1 >> _full/kinox/kinox.a-z.list
	fi
done

#cp -a _full/kinox/* /var/www/atemio/web/mediathek/kinox
