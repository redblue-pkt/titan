#!/bin/bash
#

rm cache.*
rm _liste
rm -rf _full/kinox
mkdir -p _full/kinox/streams
piccount=0
main_list="Cine-Films Popular-Movies Popular-Documentations Popular-Series Latest-Documentations Latest-Series"
echo main_list $main_list
piccount=0

for ROUND0 in $main_list; do
	wget --no-check-certificate "http://kinox.to/$ROUND0.html" -O cache.main.next.list

	main_next_list=`cat cache.main.next.list | grep /Stream/ | sed 's!/Stream/!\n/Stream/!' | grep ^/Stream/ | cut -d '"' -f1  | cut -d "'" -f1 | sort -um`
	
	if [ $ROUND0 == "Popular-Series" ]; then
		main_next_list="$main_next_list /Stream/Die_Gummibaerenbande.html /Stream/Touch-1.html"
	fi
	
#	main_next_list="/Stream/Die_Simpsons.html"
	
	skip=0

	echo main_next_list $main_next_list
	lokaltest=0
	if [ $lokaltest == 1 ];then
		main_next_list="/Stream/Touch-1.html"
	fi
	
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

		URL="http://atemio.dyndns.tv/mediathek/kinox/streams/kinox.$searchname.list"
	
		LINE="$TITLE#$URL#$PIC#kinox_$piccount.jpg#KinoX#0"
		if [ ! -z "$TITLE" ]; then
			echo $LINE >> cache.kinox.$filename.titanlist
			echo $LINE >> cache.kinox.titanlist			
		fi
			
		echo cache."$filename".list
		echo filename=$filename
#		exit
		skip1=0

		for ROUND2 in $hosterlist; do
			if [ $ROUND2 != "Sockshare.com" ] && [ $ROUND2 != "Putlocker.com" ] && [ $ROUND2 != "FileNuke.com" ];then
				echo error $ROUND2 not supported 
				continue
			fi
			piccount=`expr $piccount + 1`
			echo round2 $ROUND2
#			files=`cat cache."$filename".list | grep $searchname | grep Hoster= | tr ";" "\n" | grep ^Hoster= | sed 's/&/=/' | sed 's/"/=/' | grep $ROUND2 | cut -d "=" -f2`
			files=`cat cache."$filename".list | grep $searchname | grep $ROUND2 | tr ' ' '\n' | grep ^rel= | cut -d '"' -f2 | sed "s/amp;//" | sed "s/amp;//" | sed "s/amp;//" | sed "s/amp;//"`
			echo files $files	
			if [ ! -z "$files" ];then
	
				PIC="http://atemio.dyndns.tv/mediathek/menu/$ROUND2.jpg"
#				TITLE="$ROUND2 ($files)"	
				TITLE="$ROUND2"
				URL=http://$ROUND2

#NOTICE: http://kinox.to/aGET/List/?bSortable_4=false&bSortable_5=false&bSortable_6=true&bSortable_0=true&additional=%7B%22fType%22%3A%22series%22%2C%22fLetter%22%3A%22C%22%7D&bSortable_2=true&bSortable_3=false&sSortDir_0=asc&sEcho=1&iDisplayStart=0&iSortingCols=1&sColumns=&iColumns=7&iDisplayLength=25&iSortCol_0=2&bSortable_1=true


				seriesid=`cat cache."$filename".list | grep ';SeriesID=' | sed 's/SeriesID=/\nSeriesID"/' | grep ^SeriesID | cut -d'"' -f2`
				for ROUND4 in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24; do	
#				for ROUND4 in 1 2; do
					checkserie=`cat cache."$filename".list | grep "<option value=\"$ROUND4\" rel=" | cut -d'"' -f2`
					folgen=`cat cache."$filename".list | grep "<option value=\"$ROUND4\" rel=" | cut -d'"' -f4 | tr ',' ' '`
					
#					if [ ! -z "$checkserie" ]; then
					if [ ! -z "$seriesid" ]; then					
						echo checkserie $checkserie
						echo folgen $folgen
						echo seriesid $seriesid
						for ROUND3 in $folgen; do
							wget --no-check-certificate "http://kinox.to/aGET/MirrorByEpisode/&?Addr=$searchname&SeriesID=$seriesid&Season=$checkserie&Episode=$ROUND3" -O cache.$searchname.$checkserie.$ROUND3.list
							files=`cat cache.$searchname.$checkserie.$ROUND3.list | sed 's/rel=/\nrel=/' | grep ^rel= | grep $ROUND2 | cut -d'"' -f2 | sed "s/amp;//" | sed "s/amp;//" | sed "s/amp;//" | sed "s/amp;//"`
	
							echo files $files
							echo ROUND2 $ROUND2
	
							ls cache.$searchname.$checkserie.$ROUND3.list
							wget --no-check-certificate "http://kinox.to/aGET/Mirror/$files" -O cache.$searchname.$checkserie.$ROUND3.$ROUND2.list
							rm -rf cache.url		
							cat cache.$searchname.$checkserie.$ROUND3.$ROUND2.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g' > cache.url
			
							URL=`cat cache.url`
							FILE=`echo $URL | tr "/" "\n" | tail -n1`			
							URL="$URL;$FILE;$ROUND2"
							
							echo TITLE=$TITLE
							echo URL=$URL
							echo TITLE=$TITLE
							
							TITLEFOLGE="Session $checkserie Folge $ROUND3"
							URLFOLGE="http://atemio.dyndns.tv/mediathek/kinox/streams/kinox.$searchname.$checkserie.$ROUND3.list"
							PICFOLGE="$checkserie.$ROUND3.jpg"
	
							LINE="$TITLEFOLGE#$URLFOLGE#$PICFOLGE#kinox_$checkserie.$ROUND3.jpg#KinoX#0"
							if [ `cat _full/kinox/streams/kinox.$searchname.list | grep "$LINE" | wc -l` -eq 0 ]; then 
								echo $LINE >> _full/kinox/streams/kinox.$searchname.list
							fi
	
							if [ $ROUND2 == "Sockshare.com" ] || [ $ROUND2 == "Putlocker.com" ];then		
								LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#14"
							elif [ $ROUND2 == "FileNuke.com" ];then		
								LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#15"
							else
								LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#16"
							fi
							echo $LINE >> _full/kinox/streams/kinox.$searchname.$checkserie.$ROUND3.list					
						done
					else
						wget --no-check-certificate "http://kinox.to/aGET/Mirror/$files" -O cache.$searchname.$ROUND2.list
				
						rm -rf cache.url
						cat cache.$searchname.$ROUND2.list | sed 's!http:!\nhttp:!' | sed 's!" target=!\n!' | grep ^http: | sed 's:\\::g' > cache.url
		
						URL=`cat cache.url`
						FILE=`echo $URL | tr "/" "\n" | tail -n1`			
						URL="$URL;$FILE;$ROUND2"
						
						echo TITLE=$TITLE
						echo URL=$URL
						echo TITLE=$TITLE
	
						if [ $ROUND2 == "Sockshare.com" ] || [ $ROUND2 == "Putlocker.com" ];then		
							LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#14"
						elif [ $ROUND2 == "FileNuke.com" ];then		
							LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#15"
						else
							LINE="$TITLE#$URL#$PIC#kinox_$ROUND2.jpg#KinoX#16"
						fi
						echo $LINE >> _full/kinox/streams/kinox.$searchname.list			
						echo break
						break
					fi
				done
			fi
		done
		
		cat cache.kinox.$filename.titanlist | sort -u >> _full/kinox/streams/kinox.$ROUND0.list	
	done
done

cat cache.kinox.titanlist | sort -u > _full/kinox/kinox.all-sorted.list
cat cache.kinox.category.titanlist | sort -u > _full/kinox/kinox.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.kinox.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.kinox.titanlist | grep ^"$ROUND" > cache.kinox.titanlist."$ROUND"
		cat cache.kinox.titanlist."$ROUND" | sort -um > _full/kinox/streams/kinox.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/kinox/streams/kinox."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#KinoX#1 >> _full/kinox/kinox.a-z.list
	elif [ `cat cache.kinox.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.kinox.titanlist | grep ^"$ROUND" > cache.kinox.titanlist."$ROUND"
		cat cache.kinox.titanlist."$ROUND" | sort -um > _full/kinox/streams/kinox.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/kinox/streams/kinox."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#KinoX#1 >> _full/kinox/kinox.a-z.list
	fi
done
#cp -a _full/kinox/* /var/www/atemio/web/mediathek/kinox
exit
#done
