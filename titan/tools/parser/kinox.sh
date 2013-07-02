#!/bin/bash
#

LIST="/Stream/"
rm file.test*
rm movieliste.log

for CASE in A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	wget "http://kinox.to/aGET/List/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=1&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22movie%22%2C%22fLetter%22%3A%22$CASE%22%7D" -O file.test.$CASE
	LINES=`cat file.test.$CASE | tr ',' '\n' | grep iTotalDisplayRecords | cut -d '"' -f4`
	echo LINES $LINES

	if [ "$LINES" == " " ] || [ -z "$LINES" ] || [ "$LINES" == 0 ]; then
		rm file.test.$CASE
		break
		ls file.test.$CASE
		exit
	fi

	if [ $LINES -gt 1000 ]; then
		LINES=1000
	fi

	max=`expr $LINES - 4`
	echo max $max
	
	TMPLIST=`cat file.test.$CASE | tr '],[' '\n' | grep 'Stream' | cut -d '/' -f3 | cut -d'\\' -f1`
	LIST="$TMPLIST"
	
	count=0
	echo count $count
	echo max $max
	
	while [ $count -lt $max ]
	do
		echo "count $count"
		count=`expr $count + 1`
		wget "http://kinox.to/aGET/List/?sEcho=1&iColumns=10&sColumns=&iDisplayStart=$count&iDisplayLength=50&iSortingCols=1&iSortCol_0=5&sSortDir_0=asc&bSortable_0=true&bSortable_1=true&bSortable_2=true&bSortable_3=false&bSortable_4=false&bSortable_5=false&bSortable_6=true&additional=%7B%22fType%22%3A%22movie%22%2C%22fLetter%22%3A%22$CASE%22%7D" -O file.test.$CASE.$count
		TMPLIST=`cat file.test.$CASE.$count | tr '],[' '\n' | grep 'Stream' | cut -d '"' -f3 | tr '\\' ' ' | sed 's/ \+//g'`
		echo $TMPLIST | tr ' ' '\n' >> file.test.tmpliste
#ls file.test.$CASE.$count
#exit
#		if [ $count == 1 ]; then
#			break
#		fi
	done
done

cat file.test.tmpliste | sort -u > movieliste.log

#exit
##############

rm cache.*
rm _liste
rm -rf _full/kinox
mkdir -p _full/kinox/streams
piccount=0

main_list="Cine-Films Popular-Movies Popular-Documentations Popular-Series Latest-Documentations Latest-Series Movies"
#main_list="Movies"

echo main_list $main_list
piccount=0
counttt=0
for ROUND0 in $main_list; do
	wget --no-check-certificate "http://kinox.to/$ROUND0.html" -O cache.main.next.list
#exit
	main_next_list=`cat cache.main.next.list | grep /Stream/ | sed 's!/Stream/!\n/Stream/!' | grep ^/Stream/ | cut -d '"' -f1  | cut -d "'" -f1 | sort -um`
	
	if [ $ROUND0 == "Popular-Series" ]; then
		main_next_list="$main_next_list /Stream/Die_Gummibaerenbande.html /Stream/Touch-1.html /Stream/Alcatraz_german_subbed.html /Stream/Battlestar_Galactica.html /Stream/24.html /Stream/Heroes.html /Stream/Star_Wars_The_Clone_Wars-1.html /Stream/Star_Wars_The_Clone_Wars_german_subbed.html /Stream/TRON_Der_Aufstand.html /Stream/TRON_Uprising.html"
	fi

	if [ $ROUND0 == "Movies" ]; then
		main_next_list=`cat movieliste.log | tr '\n' ' '`
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
	LINE="$TITLE#$URL#$PIC#kinox_$piccount.jpg#KinoX#3"
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
		LANG=`cat cache."$filename".list | grep 'alt="language" src="/gr/sys/lng' | sed 's!alt="language" src="/gr/sys/lng/!\n!' | tail -n1 |cut -d"." -f1`
	 	LANGTXT=" (??)"

		if [ ! -z $LANG ];then
			if [ $LANG = 1 ];then
			 	LANGTXT=" (de)"
			elif [ $LANG = 2 ];then
			 	LANGTXT=" (en)"
			else
			 	LANGTXT=" (??)"
			fi
		fi
		
		echo cache."$filename".list
		echo filename=$filename
		echo picname=$picname
		echo PIC=$PIC
		echo TITLE=$TITLE
		echo LANGTXT=$LANGTXT

		URL="http://kinox.to/$ROUND1"
	
		LINE="$TITLE$LANGTXT#$URL#$PIC#kinox_$piccount.jpg#KinoX#22"
		if [ ! -z "$TITLE" ]; then
			echo $LINE >> cache.kinox.$filename.titanlist
			echo $LINE >> cache.kinox.titanlist			
		fi
			
		echo cache."$filename".list
		echo filename=$filename
#		counttt=`expr $counttt + 1`
#		if [ $counttt = 2 ]; then
#			exit
#		fi
		skip1=0
				
		cat cache.kinox.$filename.titanlist | sort -u >> _full/kinox/streams/kinox.$ROUND0.list	
	done
done

cat cache.kinox.titanlist | sort -u > _full/kinox/streams/kinox.all-sorted.list
cat cache.kinox.category.titanlist | sort -u > _full/kinox/kinox.category.list

for ROUND in 0 1 2 3 4 5 6 7 8 9 A B C D E F G H I J K L M N O P Q R S T U V W X Y Z; do
	filename=`echo "$ROUND" | tr 'A-Z' 'a-z'`
	if [ `cat cache.kinox.titanlist | grep ^"$ROUND" | wc -l` -gt 0 ];then
		cat cache.kinox.titanlist | grep ^"$ROUND" > cache.kinox.titanlist."$ROUND"
		cat cache.kinox.titanlist."$ROUND" | sort -um > _full/kinox/streams/kinox.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/kinox/streams/kinox."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#KinoX#3 >> _full/kinox/kinox.a-z.list
	elif [ `cat cache.kinox.titanlist | grep ^"$filename" | wc -l` -gt 0 ];then
		cat cache.kinox.titanlist | grep ^"$filename" > cache.kinox.titanlist."$ROUND"
		cat cache.kinox.titanlist."$ROUND" | sort -um > _full/kinox/streams/kinox.`echo "$ROUND" | tr 'A-Z' 'a-z'`.list
		echo `echo "$ROUND" | tr 'A-Z' 'a-z'`"#http://atemio.dyndns.tv/mediathek/kinox/streams/kinox."`echo "$ROUND" | tr 'A-Z' 'a-z'`".list#http://atemio.dyndns.tv/mediathek/menu/`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#"`echo "$ROUND" | tr 'A-Z' 'a-z'`.jpg#KinoX#3 >> _full/kinox/kinox.a-z.list
	fi
done
rm cache.*
#cp -a _full/kinox/* /var/www/atemio/web/mediathek/kinox
exit
#done

